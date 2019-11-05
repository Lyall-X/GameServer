#include "Shared.hpp"

int8 Network::sPacketBuffer_0[PACKET_MAX_LENGTH] = { 0 };
int8 Network::sPacketBuffer_1[PACKET_MAX_LENGTH] = { 0 };

class WinNet
{
public:
	WinNet()
	{
#ifdef WIN32
		WSADATA data;
		WSAStartup(0x0202, &data);
#endif
	}
	~WinNet()
	{
#ifdef WIN32
		WSACleanup();
#endif // WIN32
	}
protected:
	bool mStart;
};

Network::Network()
{
	INSTANCE(WinNet);
	mIOCP = new IOCP(this);
}

Network::~Network()
{
	SAFE_DELETE(mIOCP);
}

void Network::update(float32 time, float32 dealy)
{
	mIOCP->loop();
}

void Network::Destroy()
{
	mIOCP->Destory();
}

SocketListener* Network::listen(int16 port, const std::string host /* = "" */)
{
	return mIOCP->listen(host, port);
}

SocketClient* Network::connect(const std::string& host, int16 port)
{
	return mIOCP->connect(host, port);
}

void Network::closesocket(int32 socketId)
{
	mIOCP->PushQueueClose(socketId);
}

Socket* Network::newSocket()
{
	return new Socket;
}

SocketClient* Network::newSocketClient()
{
	return new SocketClient;
}

SocketListener* Network::newSocketListener()
{
	return new SocketListener;
}

void Network::postSend(Socket* socket, Packet& packet)
{
	mIOCP->postSend(socket, packet);
}

void Network::postSend(Socket* socket, void* dataBuffer, int dataCount)
{
	mIOCP->postSend(socket, dataBuffer, dataCount);
}

void Network::postSendEncode(Socket* socket, void* dataBuffer, int dataCount)
{
	mIOCP->postSendEncode(socket, dataBuffer, dataCount);
}



SocketListener* Network::AddListener(SocketListener* listener)
{
	mMapListener.insert(std::make_pair(listener->getSocketId(), listener));
	return listener;
}

SocketListener* Network::FindListener(uint32 socketId)
{
	auto itr = mMapListener.find(socketId);
	if (itr != mMapListener.end()) return itr->second;

	return NULL;
}

void Network::DelListener(uint32 socketId)
{
	auto itr = mMapListener.find(socketId);
	if (itr != mMapListener.end())
	{
		delete itr->second;
		mMapListener.erase(itr);
	}
}

SocketClient* Network::AddClient(SocketClient* client)
{
	mMapClient[client->getSocketId()] = client;
	return client;
}

SocketClient* Network::FindClient(uint32 socketId)
{
	auto itr = mMapClient.find(socketId);
	if (itr != mMapClient.end()) return itr->second;
	return NULL;
}

void Network::DelClient(uint32 socketId)
{
	auto itr = mMapClient.find(socketId);
	if (itr != mMapClient.end())
	{
		delete itr->second;
		mMapClient.erase(itr);
	}
}

Socket* Network::AddSocket(Socket* socket)
{
	mMapSocket[socket->getSocketId()] = socket;
	return socket;
}

Socket* Network::FindSocket(uint32 socketId)
{
	auto itr = mMapSocket.find(socketId);
	if (itr != mMapSocket.end()) return itr->second;
	return NULL;
}

void Network::DelSocket(uint32 socketId)
{
	auto itr = mMapSocket.find(socketId);
	if (itr != mMapSocket.end())
	{
		delete itr->second;
		mMapSocket.erase(itr);
	}
}

void Network::OnAccept(SocketListener* listener, Socket* socket)
{
	SocketEvent event;
	event.event = SocketEvent::ACCEPT;
	event.socket = socket;
	listener->addSocket(socket);
	socket->Dispatch(event);
	listener->Dispatch(event);
}

void Network::OnConnect(SocketClient* client, Socket* socket)
{
	SocketEvent event;
	event.event = SocketEvent::CONNECT;
	event.socket = socket;

	socket->Dispatch(event);
	client->Dispatch(event);
}

void Network::OnRecv(Socket* socket)
{
	CircularBuffer* readBuffer = socket->readStream;
	SocketAngent* angent = socket->angent;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	if (readBuffer->get_space_length() <= 0)
	{
		this->closesocket(socket->getSocketId());
		// 读取数据的环形缓冲不已满;
		LOG_ERROR("readBuffer full");
		return;
	}

	if (readBuffer->get_space_length() < (uint32)ioOverlapped.dwBytesTransferred)
	{
		this->closesocket(socket->getSocketId());
		LOG_ERROR("readBuffer data not enough");
		return;
	}
	readBuffer->Push(ioOverlapped.dataBuffer, ioOverlapped.dwBytesTransferred);
	//socket->packetCount = 0;
	while (true)
	{
		bool recvPacket = false;
		int32 packetCount = 0;
		if (readBuffer->Read(&packetCount, sizeof(packetCount), sizeof(packetCount)))
		{
			packetCount = Shared::ntohl(packetCount);
			if (readBuffer->get_data_length() >= (uint32)packetCount)
			{
				int8* packetBuffer = sPacketBuffer_0;
				if (readBuffer->Pop(packetBuffer, PACKET_MAX_LENGTH, packetCount))
				{
					const int8* data = packetBuffer + sizeof(int32);
					int32 len = packetCount - sizeof(int32);

					if (len > PACKET_MAX_LENGTH)
					{
						this->closesocket(socket->getSocketId());
						return;
					}

					//socket->packetCount++;
					SocketEvent event;
					event.event = SocketEvent::RECV;
					event.socket = socket;
					event.data = (void*)data;
					event.count = len;

					angent->Dispatch(event);
					socket->Dispatch(event);
					recvPacket = true;
				}
				else
				{
					this->closesocket(socket->getSocketId());
					return;
				}
			}
		}

		if (!recvPacket)break;
	}
}

void Network::OnSend(Socket* socket)
{
	SocketAngent* angent = socket->angent;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	SocketEvent event;
	event.event = SocketEvent::SEND;
	event.socket = socket;
	event.data = ioOverlapped.dataBuffer;
	event.count = ioOverlapped.dwBytesTransferred;

	angent->Dispatch(event);
	socket->Dispatch(event);
}

void Network::OnExit(Socket* socket)
{
	int32 socketId = socket->getSocketId();

	SocketAngent* angent = socket->angent;

	SocketClient* client = FindClient(angent->getSocketId());
	SocketListener* listener = FindListener(angent->getSocketId());

	SocketEvent event;
	event.event = SocketEvent::EXIT;
	event.socket = socket;

	if (client)
		client->Dispatch(event);

	if (listener)
		listener->Dispatch(event);

	socket->Dispatch(event);

	DelSocket(socketId);

	if (client)
		DelClient(socketId);

	if (listener)
		listener->delSocket(socketId);
}

void Network::OnException(Socket* socket)
{
	uint32 socketId = socket->getSocketId();
	SocketAngent* angent = socket->angent;
	SocketEvent se;
	se.event = SocketEvent::EXCEPTION;
	socket->Dispatch(se);
	angent->Dispatch(se);
	DelSocket(socketId);
	DelClient(socketId);
}

bool Network::unCompression(char* input, int inCount, char* output, int& outCount)
{
	char* bytes = NULL;
	Compression::zlib_UnCompression((char*)input, inCount, bytes, outCount);
	if (outCount > PACKET_MAX_LENGTH || outCount <= 0)
	{
		LOG_ERROR("error: %s", __FUNCTION__);
		delete bytes;
		return false;
	}
	memcpy_s(output, PACKET_MAX_LENGTH, bytes, outCount);
	delete bytes;
	return true;
}

bool Network::compression(char* input, int inCount, char* output, int& outCount)
{
	char* bytes = NULL;
	Compression::zlib_Compression((char*)input, inCount, bytes, outCount);
	if (outCount > PACKET_MAX_LENGTH || outCount <= 0)
	{
		LOG_ERROR("error: %s", __FUNCTION__);
		delete bytes;
		return false;
	}
	memcpy_s(output, PACKET_MAX_LENGTH, bytes, outCount);
	delete bytes;
	return true;
}
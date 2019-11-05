#include "Shared.hpp"

IOCP::IOCP(Network* network)
{
	mNetwork = network;
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	mWorkerThreadsCount = sysInfo.dwNumberOfProcessors * 2 + 1;
	mWorkerThreads = new Threader[mWorkerThreadsCount];
	for (uint32 i = 0; i < mWorkerThreadsCount; ++i){
		mWorkerThreads[i].CreateThread((ThreadCallBack)&IOCP::WorkerThread, this);
	}
}

IOCP::~IOCP()
{
	Destory();
}

void IOCP::Destory()
{
	for (uint32 i = 0; i < mWorkerThreadsCount; ++i)
	{
		mWorkerThreads[i].Eixt();
		PostQueuedCompletionStatus(mIOCP, 0, 0, NULL);
	}
	SAFE_DELETE_ARRAY(mWorkerThreads);
	mWorkerThreadsCount = 0;
	mNetwork = NULL;
}

SocketListener* IOCP::listen(const std::string& host, short port)
{
	uint32 socketId = (uint32)WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	SocketListener* listener = mNetwork->newSocketListener();
	
	Socket* socket = mNetwork->newSocket();
	socket->angent = listener;
	socket->network = mNetwork;

	socket->setSocketId(socketId);
	listener->setSocket(socket);
	listener->setSocketId(socketId);

	CreateIoCompletionPort((HANDLE)socketId, mIOCP, (ULONG_PTR)socketId, 0);

	SOCKADDR_IN& addr = listener->getSockaddr();
	addr = {};

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if (0 != ::bind(socketId, (SOCKADDR*)&addr, sizeof(addr)))
	{
		delete listener;
		delete socket;
		return NULL;
	}
	if (0 != ::listen(socketId, 5))
	{
		delete listener;
		delete socket;
		return NULL;
	}

	GUID guidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes = 0;
	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx), &listener->mPfn, sizeof(listener->mPfn),
		&dwBytes, NULL, NULL);

	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs), &listener->mPfnSocketAddrs, sizeof(listener->mPfnSocketAddrs),
		&dwBytes, NULL, NULL);

	for (int32 i = 0; i < 10; i++)
	{
		PostAccept(listener);
	}
	mNetwork->AddSocket(socket);
	mNetwork->AddListener(listener);
	return listener;
}

SocketClient* IOCP::connect(const std::string& host, short port)
{
	uint32 socketId = (uint32)WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);

	SocketClient* client = mNetwork->newSocketClient();
	client->setSocketId(socketId);

	SOCKADDR_IN& addr = client->getSockaddr();
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	addr.sin_port = htons(port);

	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = INADDR_ANY;
	local.sin_port = 0;
	if (SOCKET_ERROR == bind(socketId, (LPSOCKADDR)&local, sizeof(local)))
	{
		printf("绑定套接字失败!\r\n");
		SAFE_DELETE(client);
		return NULL;
	}

	GUID guidConnectEx = WSAID_CONNECTEX;
	DWORD dwBytes = 0;
	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnectEx, sizeof(guidConnectEx), &client->mPfn, sizeof(client->mPfn),
		&dwBytes, NULL, NULL);
	if (!PostConnect(client))
	{
		SAFE_DELETE(client);
		return NULL;
	}

	mNetwork->AddClient(client);

	return client;
}

bool IOCP::loop()
{

	while (true)
	{
		mMutex.lock();
		uint32 queueSize = (uint32)mQueueEvent.size();
		mMutex.unlock();
		if (queueSize == 0) break;

		mMutex.lock();
		QueueResponse response = mQueueEvent.front();
		mQueueEvent.pop();
		mMutex.unlock();

		
		Socket* socket = mNetwork->FindSocket(response.socketId);
		if (socket == NULL) continue;
		response.ioOverlapped->dwBytesTransferred = response.BytesTransferred;
		response.ioOverlapped->dwResult = response.result;
		IO_OVERLAPPED* ioOverlapped = response.ioOverlapped;
		SocketAngent* angent = socket->angent;

		switch (response.ioOverlapped->ioState)
		{
		case IOState_Accept:
			DoAccept(angent->getSocketId(), ioOverlapped->socket);
			break;
		case IOState_Connect:
			DoConnect(angent->getSocketId(), ioOverlapped->socket);
			break;
		case IOState_Recv:
			DoRead(ioOverlapped->socket);
			break;
		case IOState_Send:
			DoWrite(ioOverlapped->socket);
			break;
		default:
			LOG_ERROR(__FUNCTION__);
			break;
		}
	}

	while (mQueueClose.size() > 0)
	{
		auto itr = mQueueClose.begin();
		uint32 socketId = (*itr);
		DoExit(mNetwork->FindSocket(socketId));
		mQueueClose.erase(itr);
	}
	return true;
}



void IOCP::postSend(Socket* socket, Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	postSend(socket, in.datas(), in.wpos());
}

void IOCP::postSend(Socket* socket, void* dataBuffer, int dataCount)
{
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;

	object_bytes packet;
	packet.len = dataCount + sizeof(int);
	if (packet.len > PACKET_MAX_LENGTH)
	{
		LOG_ERROR("send max length:%d", PACKET_MAX_LENGTH);
		PushQueueClose(socket->getSocketId());
		return;
	}
	if (dataCount <= 0)
	{
		LOG_ERROR("send dataCount <= 0");
		PushQueueClose(socket->getSocketId());
		return;
	}
	packet.ptr = new int8[packet.len];

	BinaryStream in((void*)packet.ptr, packet.len);
	in << packet.len;
	in.write(dataBuffer, dataCount);

	socket->sendQueue.push(packet);
	if (!socket->startSend)
		PostWrite(socket);
}

void IOCP::postSendEncode(Socket* socket, void* dataBuffer, int dataCount)
{

}

bool IOCP::PostAccept(SocketListener* listener)
{
	Socket* socket = mNetwork->newSocket();
	socket->angent = listener;
	socket->network = mNetwork;
	uint32 socketId = (uint32)WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	socket->setSocketId(socketId);
	socket->postCount++;

	LPFN_ACCEPTEX pfn = listener->mPfn;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	ioOverlapped.overlapped = {};
	ioOverlapped.socket = socket;
	ioOverlapped.ioState = IOState_Accept;
	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	ioOverlapped.wBuffer.len = ioOverlapped.dataBufferCount;

	DWORD dwBytes = 0;
	DWORD dwLocalAddressLength = sizeof(sockaddr_in) + 16;
	DWORD dwRemoteAddressLength = sizeof(sockaddr_in) + 16;
	DWORD dwReceiveDataLength = ioOverlapped.dataBufferCount - dwLocalAddressLength - dwRemoteAddressLength;

	BOOL ret = pfn(
		listener->getSocketId(),
		socket->getSocketId() ,
		ioOverlapped.dataBuffer,
		0,
		dwLocalAddressLength, 
		dwRemoteAddressLength,
		&dwBytes,
		&(ioOverlapped.overlapped));

	if (!ret && WSA_IO_PENDING != WSAGetLastError())
	{
		delete socket;
		return false;
	}

	mNetwork->AddSocket(socket);
	return true;
}

bool IOCP::PostConnect(SocketClient* client)
{
	Socket* socket = mNetwork->newSocket();
	socket->angent = client;
	socket->network = mNetwork;
	socket->setSocketId(client->getSocketId());
	client->setSocket(socket);
	socket->postCount++;

	CreateIoCompletionPort((HANDLE)socket->getSocketId(), mIOCP, (ULONG_PTR)socket->getSocketId(), 0);

	LPFN_CONNECTEX pfn = client->mPfn;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	ioOverlapped.overlapped = {};
	ioOverlapped.socket = socket;
	ioOverlapped.ioState = IOState_Connect;

	sockaddr_in& addr = client->getSockaddr();
	int nLen = sizeof(SOCKADDR_IN);
	DWORD dwBytesSent = 0;
	// 重点
	BOOL bResult = pfn(
		client->getSocketId(),
		(sockaddr*)&addr,				// [in] 对方地址
		sizeof(addr),									// [in] 对方地址长度
		NULL,									// [in] 连接后要发送的内容，这里不用
		0,										// [in] 发送内容的字节数 ，这里不用
		&dwBytesSent,							// [out] 发送了多少个字节，这里不用
		&ioOverlapped.overlapped);				// [in]
	DWORD dwError = WSAGetLastError();
	if (!bResult && ERROR_IO_PENDING != dwError)
	{
		delete socket;
		return false;
	}

	std::string host = Shared::inet_ntoa(&addr.sin_addr.s_addr);
	socket->setHost(host);
	mNetwork->AddSocket(socket);
	return true;
}

bool IOCP::PostRead(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	ioOverlapped.overlapped = {};
	ioOverlapped.ioState = IOState_Recv;
	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	ioOverlapped.wBuffer.len = ioOverlapped.dataBufferCount;
	DWORD dwBufferCount = 1, dwRecvBytes = 0, Flags = 0;
	socket->postCount++;

	int32 result = WSARecv(socket->getSocketId(), &ioOverlapped.wBuffer, dwBufferCount, &dwRecvBytes, &Flags, &ioOverlapped.overlapped, NULL);
	DWORD dwError = WSAGetLastError();
	if ((result == SOCKET_ERROR) && (WSA_IO_PENDING != dwError)) {
		PushQueueClose(socket->getSocketId());
		return false;
	}
	return true;
}

bool IOCP::PostWrite(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;
	ioOverlapped.socket = socket;
	socket->startSend = false;
	socket->postCount++;
	std::queue<object_bytes>& sendQueue = socket->sendQueue;

	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	if (sendQueue.size() <= 0) return false;

	DWORD dwBufferCount = 1, dwRecvBytes = 0, Flags = 0;
	char* wBuffer = ioOverlapped.dataBuffer;
	int32 sendSize = 0;
	while (sendQueue.size() > 0)
	{
		object_bytes& packet = sendQueue.front();
		if (sendSize + packet.len > ioOverlapped.dataBufferCount) {
			break;
		}

		memcpy(wBuffer + sendSize, packet.ptr, packet.len);
		sendSize += packet.len;
		sendQueue.pop();
		SAFE_DELETE_ARRAY(packet.ptr);
	}
	if (sendSize == 0)
	{
		PushQueueClose(socket->getSocketId());
		return false;
	}
	socket->startSend = true;
	ioOverlapped.overlapped = {};
	ioOverlapped.wBuffer.len = sendSize;
	ioOverlapped.ioState = IOState_Send;
	int32 result = WSASend(socket->getSocketId(), &ioOverlapped.wBuffer, dwBufferCount, &dwRecvBytes, Flags, &ioOverlapped.overlapped, NULL);
	DWORD dwError = WSAGetLastError();
	if ((result == SOCKET_ERROR) && (WSA_IO_PENDING != dwError)) {
		PushQueueClose(socket->getSocketId());
		return false;
	}
	return true;
}

void IOCP::DoAccept(uint32 socketId, Socket* socket)
{
	socket->postCount--;
	SocketListener* listener = mNetwork->FindListener(socketId);
	if (listener == NULL)  return;
	CreateIoCompletionPort((HANDLE)socket->getSocketId(), mIOCP, (ULONG_PTR)socket->getSocketId(), 0);
	
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	SOCKADDR_IN* remote = NULL;
	SOCKADDR_IN* local = NULL;

	int32 remoteLen = sizeof(SOCKADDR_IN);
	int32 localLen = sizeof(SOCKADDR_IN);
	uint32 dwLocalAddressLength = sizeof(sockaddr_in) + 16;
	uint32 dwRemoteAddressLength = sizeof(sockaddr_in) + 16;
	uint32 dwReceiveDataLength = ioOverlapped.dataBufferCount - dwLocalAddressLength - dwRemoteAddressLength;
	sockaddr_in addr;
	int32 addrlen = sizeof(addr);
	getsockname(socket->getSocketId(), (sockaddr*)&addr, &addrlen);
	listener->mPfnSocketAddrs(
		ioOverlapped.dataBuffer,
		0,
		dwLocalAddressLength,
		dwRemoteAddressLength,
		(LPSOCKADDR*)&local, &localLen,
		(LPSOCKADDR*)&remote, &remoteLen);

	std::string host = Shared::inet_ntoa(&remote->sin_addr.s_addr);
	socket->setHost(host);

	mNetwork->OnAccept(listener, socket);
	PostRead(socket);

	PostAccept(listener);
}

void IOCP::DoConnect(uint32 socketId, Socket* socket)
{
	socket->postCount--;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	SocketAngent* angent = socket->angent;

	if (ioOverlapped.dwResult == 0 && ioOverlapped.dwBytesTransferred == 0)
	{
		mNetwork->OnException(socket);
		return;
	}
	mNetwork->OnConnect((SocketClient*)angent, socket);
	PostRead(socket);
}

void IOCP::DoRead(Socket* socket)
{
	socket->postCount--;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	if (ioOverlapped.dwBytesTransferred <= 0)
	{
		PushQueueClose(socket->getSocketId());
		return;
	}

	mNetwork->OnRecv(socket);
	PostRead(socket);
}

void IOCP::DoWrite(Socket* socket)
{
	socket->postCount--;
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;
	if (ioOverlapped.dwBytesTransferred <= 0)
	{
		PushQueueClose(socket->getSocketId());
		return;
	}
	mNetwork->OnSend(socket);
	PostWrite(socket);
}

void IOCP::DoExit(Socket* socket)
{
	if (socket == NULL) return;
	mNetwork->OnExit(socket);
}


void IOCP::PushQueueResponse(QueueResponse& response)
{
	mMutex.lock();
	mQueueEvent.push(response);
	mMutex.unlock();
}

void IOCP::PushQueueClose(uint32 socketId)
{
	auto itr = mQueueClose.find(socketId);
	if (itr != mQueueClose.end()) return;
	uint32 tSocketId = socketId;
	mQueueClose.insert(socketId);
}

uint32 IOCP::WorkerThread(Threader& threader)
{
	while (threader.Active())
	{
		Shared::Sleep(1);
		OVERLAPPED* overlapped = NULL;
		IO_OVERLAPPED* ioOverlapped = NULL;
		BOOL dwResult = false;
		DWORD dwBytesTransferred = 0;
		SocketAngent* angent = NULL;
		uint32 socketId = 0;

		dwResult = GetQueuedCompletionStatus(mIOCP, &dwBytesTransferred, (PULONG_PTR)&socketId, &overlapped, INFINITE);
		ioOverlapped = (IO_OVERLAPPED*)CONTAINING_RECORD(overlapped, IO_OVERLAPPED, overlapped);
		QueueResponse response;
		response.ioOverlapped = ioOverlapped;
		response.socketId = socketId;
		response.result = dwResult;
		response.BytesTransferred = dwBytesTransferred;
		PushQueueResponse(response);
	}
	return 0;
}

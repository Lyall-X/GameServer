#include "Shared.hpp"

SocketListener::SocketListener():
mPfn(NULL),
mPfnSocketAddrs(NULL)
{
}

SocketListener::~SocketListener()
{
	clearSocket();
}

Socket* SocketListener::addSocket(Socket* sock)
{
	if (getSocket(sock->getSocketId()))
		return NULL;

	mSockets.insert(std::make_pair(sock->getSocketId(), sock));
	return sock;
}

void SocketListener::delSocket(int32 socketId)
{
	std::map<int32, Socket*>::iterator itr = mSockets.find(socketId);
	if (itr != mSockets.end())
		mSockets.erase(itr);
}

Socket* SocketListener::getSocket(int32 socketId)
{
	std::map<int32, Socket*>::iterator itr = mSockets.find(socketId);
	if (itr != mSockets.end())
		return itr->second;

	return NULL;
}

void SocketListener::clearSocket()
{
	mSockets.clear();
}

void SocketListener::sendPacket(Socket* socket, Packet& packet)
{
	if (socket == NULL)
		return;

	socket->sendPacket(packet);
}

void SocketListener::sendPacket(int32 socketId, Packet& packet)
{
	Socket* socket = getSocket(socketId);
	if (socket == NULL)
		return;

	socket->sendPacket(packet);
}

void SocketListener::sendBuffer(Socket* socket, void* data, int32 count)
{
	if (socket == NULL)
		return;

	socket->sendBuffer(data, count);
}

void SocketListener::sendBuffer(int32 socketId, void* data, int32 count)
{
	Socket* socket = getSocket(socketId);
	if (socket == NULL)
		return;

	socket->sendBuffer(data, count);
}

void SocketListener::sendPacketToAll(Packet& packet)
{
	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	in << packet;
	sendBufferToAll(in.datas(), in.wpos());
}

void SocketListener::sendBufferToAll(void* data, int32 count)
{
	for (auto itr: mSockets)
		itr.second->sendBuffer(data, count);
}

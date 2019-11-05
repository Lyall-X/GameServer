#include "Shared.hpp"

SocketClient::SocketClient():
mPfn(NULL)
{

}

SocketClient::~SocketClient()
{

}

void SocketClient::sendPacket(Packet& packet)
{
	if (mSocket == NULL) return;
	mSocket->sendPacket(packet);
}

void SocketClient::sendBuffer(void* data, int32 count)
{
	if (mSocket == NULL) return;
	mSocket->sendBuffer(data, count);
}

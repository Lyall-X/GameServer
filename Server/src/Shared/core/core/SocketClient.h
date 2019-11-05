#pragma once

class SocketClient : public SocketAngent
{
	friend class Network;
public:
	SocketClient();
	~SocketClient();

	void sendPacket(Packet& packet);
	void sendBuffer(void* data, int32 count);
public:
	LPFN_CONNECTEX						mPfn;
};
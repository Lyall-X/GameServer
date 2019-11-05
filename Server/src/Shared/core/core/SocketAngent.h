#pragma once

class SocketAngent :public EventDispatcher
{
	friend class Network;
	friend class IOCP;
public:
	SocketAngent();
	~SocketAngent();
	uint32 getSocketId() { return mSocketId; }
	Socket* getSocket() { return mSocket; }
protected:
	void setSocketId(int32 socketId) { mSocketId = socketId; }
	void setSocket(Socket* s) { mSocket = s; }
	sockaddr_in& getSockaddr() { return mAddr; }
protected:
	uint32				mSocketId;
	Socket*				mSocket;
	struct sockaddr_in  mAddr;
};
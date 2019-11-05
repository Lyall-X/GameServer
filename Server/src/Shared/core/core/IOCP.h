#pragma once


class SocketAngent;
class Socket;

struct QueueResponse
{
	IO_OVERLAPPED* ioOverlapped = NULL;
	SocketAngent* angent = NULL;
	uint32 socketId = 0;
	DWORD result = 0;
	DWORD BytesTransferred = 0;
};

class IOCP : public Object
{
public:
	friend class Network;
	IOCP(Network* network);
	~IOCP();
	SocketListener* listen(const std::string& host, short port);
	SocketClient* connect(const std::string& host, short port);
	bool loop();
	void Destory();
protected:
	void postSend(Socket* socket, Packet& packet);
	void postSend(Socket* socket, void* dataBuffer, int dataCount);
	void postSendEncode(Socket* socket, void* dataBuffer, int dataCount);
protected:
	bool PostAccept(SocketListener* listener);
	bool PostConnect(SocketClient* client);
	bool PostRead(Socket* socket);
	bool PostWrite(Socket* socket);
protected:
	void DoAccept (uint32 socketId, Socket* socket);
	void DoConnect(uint32 socketId, Socket* socket);
	void DoRead(Socket* socket);
	void DoWrite(Socket* socket);
	void DoExit(Socket* socket);

	void PushQueueResponse(QueueResponse& response);
	void PushQueueClose(uint32 socketId);
protected:
	uint32 WorkerThread(Threader& threader);
protected:
	Network*								mNetwork;
	HANDLE									mIOCP;
	Mutex									mMutex;
	std::queue<QueueResponse>				mQueueEvent;
	std::set<uint32>						mQueueClose;
	Threader*								mWorkerThreads;
	uint32									mWorkerThreadsCount;
};


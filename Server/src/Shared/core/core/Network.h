#pragma once

class Socket;
class Packet;
class SocketAngent;
class SocketListener;
class SocketClient;

class Network
{
	friend class Socket;
	friend class IOCP;
public:
	Network();
	~Network();
	void update(float32 time, float32 dealy);
	void Destroy();
public:
	SocketListener* listen(int16 port, const std::string host = "");
	SocketClient* connect(const std::string& host, int16 port);
	void closesocket(int32 socketId);
protected:
	virtual Socket* newSocket();
	virtual SocketClient* newSocketClient();
	virtual SocketListener* newSocketListener();
	void postSend(Socket* socket, Packet& packet);
	void postSend(Socket* socket, void* dataBuffer, int dataCount);
	void postSendEncode(Socket* socket, void* dataBuffer, int dataCount);
protected:
	SocketListener* AddListener(SocketListener* listener);
	SocketListener* FindListener(uint32 socketId);
	void DelListener(uint32 socketId);

	SocketClient* AddClient(SocketClient* client);
	SocketClient* FindClient(uint32 socketId);
	void DelClient(uint32 socketId);

	Socket* AddSocket(Socket* socket);
	Socket* FindSocket(uint32 socketId);
	void DelSocket(uint32 socketId);

	void OnAccept(SocketListener* listener, Socket* socket);
	void OnConnect(SocketClient* client, Socket* socket);
	void OnRecv(Socket* socket);
	void OnSend(Socket* socket);
	void OnExit(Socket* socket);
	void OnException(Socket* socket);
protected:
	bool compression(char* input, int inCount, char* output, int& outCount);
	bool unCompression(char* input, int inCount, char* output, int& outCount);
protected:
	IOCP*							mIOCP;
protected:
	static							int8 sPacketBuffer_0[PACKET_MAX_LENGTH];
	static							int8 sPacketBuffer_1[PACKET_MAX_LENGTH];
protected:
	std::map<uint32, SocketListener*>		mMapListener;
	std::map<uint32, SocketClient*>			mMapClient;
	std::map<uint32, Socket*>				mMapSocket;
};

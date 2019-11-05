#pragma once

class Player;
class Socket;
class Packet;
class SocketClient;

class Session
{
public:
	Session(Socket* s);
	~Session();
public:
	static		uint32 sId;
	uint64		sessionId;
	Player*		player;
	Socket*		socket;
	void setPlayer(Player* plr) { player = plr; }
	Player* getPlayer() { return player; }
	void setSsnId(uint64 sId) { sessionId = sId; }
	uint64 getSsnId() { return sessionId; }
	void setSocket(Socket* s) { socket = s; }
	Socket* getSocket() { return socket; }
	uint32 getSocketId();

	void sendPacket(Packet& packet);
	void sendBuffer(void* data, int32 count);

	void sendPacketToWorld(Packet& packet);
	void sendBufferToWorld(void* data, int32 count);
protected:
};
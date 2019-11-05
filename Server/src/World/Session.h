#pragma once

class Player;
class Socket;
class Session
{
	friend class SessionManager;
public:
	Session(Socket* s);
	~Session();
public:
	void setPlayer(Player* plr) { mPlayer = plr; }
	Player* getPlayer() { return mPlayer; }
	void setSsnId(SessionID sId) { mSessionId = sId; }
	SessionID getSsnId() { return mSessionId; }
	void setSocket(Socket* s) { mSocket = s; }
	Socket* getSocket() { return mSocket; }
	uint32 getSocketId();
	uint64 getDownloadBytes() { return mDownloadBytes; }
	uint64 getUploadBytes() { return mUploadBytes; }

	void sendBuffer(void* data, int32 count);
	void sendPacketToDB(Packet& packet);

	void sendPacketToWorld(Packet& packet);
	void sendBufferToWorld(void* data, int32 count);
	const std::string& getHost() { return mHost; }
	void setHost(const std::string& host) { mHost = host; }
	void addDownloadBytes(uint32 bytes) { mDownloadBytes += bytes; }
	void addUploadBytes(uint32 bytes) { mUploadBytes += bytes; }
protected:
	static		uint32 sId;
protected:
	SessionID	mSessionId;
	Player*		mPlayer;
	Socket*		mSocket;
	std::string mHost;
	uint64		mDownloadBytes = 0;
	uint64		mUploadBytes = 0;
};
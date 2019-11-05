#pragma once

class Socket;
class Packet;

class SocketEvent : public Event
{
public:
	static const char* CONNECT;
	static const char* ACCEPT;
	static const char* RECV;
	static const char* SEND;
	static const char* EXIT;
	static const char* EXCEPTION;
public:
	Socket* socket;
	void* data;
	int32 count;
	Packet* packet;
};
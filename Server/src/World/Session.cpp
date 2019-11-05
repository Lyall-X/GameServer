#include "stdafx.h"

uint32 Session::sId = 0;

Session::Session(Socket* s) :
mSessionId(0),
mPlayer(NULL),
mSocket(s)
{

}

Session::~Session()
{

}

uint32 Session::getSocketId()
{
	return mSocket ? mSocket->getSocketId() : 0;
}

void Session::sendBuffer(void* data, int32 count)
{
	if (!mSocket) return;
	mSocket->sendBuffer(data, count);
	addDownloadBytes(count + sizeof(uint32));
}

void Session::sendPacketToDB(Packet& packet)
{
	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << (int8)Snd_Ssn;
	in << getSsnId();
	int32 pos = in.wpos();
	in << packetCount;
	int32 offset = in.wpos();
	in << packet;
	packetCount = in.wpos() - offset;
	packetCount = Shared::htonl(packetCount);
	in.push(pos, &packetCount, sizeof(int32));
	GetDBServer()->sendBuffer(in.datas(), in.wpos());
}

void Session::sendPacketToWorld(Packet& packet)
{
	if (!mSocket) return;

	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream bytes(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	bytes << getSsnId();
	int32 pos = bytes.wpos();
	bytes << packetCount;
	int32 offset = bytes.wpos();
	bytes << packet;
	packetCount = bytes.wpos() - offset;
	packetCount = Shared::htonl(packetCount);

	bytes.push(pos, &packetCount, sizeof(int32));
	sendBuffer(bytes.datas(), bytes.wpos());
}

void Session::sendBufferToWorld(void* data, int32 count)
{
	if (!mSocket) return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  bytes(output, PACKET_MAX_LENGTH);
	bytes << getSsnId();
	bytes << count;
	bytes.write(data, count);
	sendBuffer(bytes.datas(), bytes.wpos());
}


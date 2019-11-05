#include "stdafx.h"

void Player::sendPacket(Packet& packet) 
{
	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << (int8)Snd_Plr;
	in << getAccId();
	int32 pos = in.wpos();
	in << packetCount;
	int32 offset = in.wpos();
	in << packet;
	packetCount = in.wpos() - offset;
	packetCount = Shared::htonl(packetCount);
	in.push(pos, &packetCount, sizeof(int32));

	mSsn->sendBuffer(in.datas(), in.wpos());
}

void Player::bindSsn(Session* ssn)
{
	ssn->setPlayer(this);
	setSession(ssn);
	setSocket(ssn->getSocket());
}

void Player::unbindSsn()
{
	mSsn->setPlayer(NULL);
	setSession(NULL);
}

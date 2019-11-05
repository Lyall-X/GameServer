#include "Shared.hpp"

#define CUR_VERSION 1

Packet::Packet(int32 msgId):
mMsgId(msgId),
mVersion(CUR_VERSION)
{
}

bool Packet::operator >> (BinaryStream& bytes)
{
	CHECK(this->OnPackHeader(bytes));
	return OnSerialize(bytes);
}

bool Packet::operator << (BinaryStream& bytes)
{
	CHECK(OnUnPackHeader(bytes));
	return OnDeserialize(bytes);
}

bool Packet::OnPackHeader(BinaryStream& bytes)
{
	CHECK(bytes << mMsgId);
	CHECK(bytes << mVersion);
	return true;
}

bool Packet::OnUnPackHeader(BinaryStream& bytes)
{
	CHECK(bytes >> mMsgId);
	CHECK(bytes >> mVersion);
	return true;
}

bool Packet::OnSerialize(BinaryStream& bytes)
{
	return true;
}

bool Packet::OnDeserialize(BinaryStream& bytes)
{
	return true;
}


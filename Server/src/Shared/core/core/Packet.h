#pragma once

class BinaryStream;
class Packet : public Object
{
public:
	Packet(int32 msgId);
	virtual int32 getMsgId() { return mMsgId; }
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);
protected:
	virtual bool OnPackHeader(BinaryStream& bytes);
	virtual bool OnUnPackHeader(BinaryStream& bytes);
	virtual bool OnSerialize(BinaryStream& bytes);
	virtual bool OnDeserialize(BinaryStream& bytes);
public:
	int32  mMsgId;
	int32  mVersion;
};

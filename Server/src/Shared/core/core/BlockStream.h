#pragma once

struct List;
class BlockStream
{
public:
	BlockStream(int32 count = 256, int32 blockCount = 32);
	BlockStream(const BlockStream& other);
	BlockStream(int8* value, int32 count);
 	virtual ~BlockStream();
	bool write(void* bytes, int32 count);
	bool read(void* bytes, int32 count);

	int32 getReadableCount();
	int32 getWriteableCount();
	void Clear();
public:
	bool operator << (const float32& value);
	bool operator << (const float64& value);
	bool operator << (const std::string& value);
	bool operator << (const int8 value);
	bool operator << (const uint8 value);
	bool operator << (const int16 value);
	bool operator << (const uint16 value);
	bool operator << (const int32 value);
	bool operator << (const uint32 value);
	bool operator << (const int64 value);
	bool operator << (const uint64 value);
	bool operator << (const Variant& value);
	bool operator << (const Object& value);

	bool operator << (BinaryStream& value);
public:
	bool operator >> (float32& value);
	bool operator >> (float64& value);
	bool operator >> (std::string& value);

	bool operator >> (int8& value);
	bool operator >> (uint8& value);
	bool operator >> (int16& value);
	bool operator >> (uint16& value);
	bool operator >> (int32& value);
	bool operator >> (uint32& value);
	bool operator >> (int64& value);
	bool operator >> (uint64& value);
	bool operator >> (Variant& value);
	bool operator >> (Object& value);
	bool operator >> (BinaryStream& value);
protected:
	List*		mBlockList;
	int32		mBlockCount;
	int32		mTotalCount;
	int32		mWriteIndex;
	int32		mReadIndex;
};
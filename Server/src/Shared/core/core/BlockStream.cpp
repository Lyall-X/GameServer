#include "Shared.hpp"

void freeValue(void* value)
{
	delete [] (char*)value;
}

BlockStream::BlockStream(int32 count /* =256 */, int32 blockCount /*= 32*/)
{
	mBlockList = listCreate();
	mBlockList->free = freeValue;

	mBlockCount = blockCount;
	int32 listCount = 0;
	if (blockCount >= count)
		listCount = 1;
	else
		listCount = count % blockCount == 0 ? count / blockCount : count / blockCount + 1;

	mTotalCount = listCount * mBlockCount;
	mWriteIndex = 0;
	mReadIndex = 0;

	for (int32 i = 0; i < listCount; ++i)
	{
		listAddNodeTail(mBlockList, new int8[mBlockCount]);
	}
}

BlockStream::BlockStream(const BlockStream& other)
{
	mBlockList = listCreate();
	mBlockList->free = freeValue;

	mBlockCount = other.mBlockCount;
	mTotalCount = other.mTotalCount;
	mWriteIndex = other.mWriteIndex;
	mReadIndex = other.mReadIndex;

	ListNode* node = other.mBlockList->head;
	while (node)
	{
		ListNode* block = listAddNodeTail(mBlockList, new int8[mBlockCount]);
		memcpy_s(block->value, mBlockCount, node->value, other.mBlockCount);
		node = node->next;
	}
}

BlockStream::BlockStream(int8* value, int32 count)
{

}

BlockStream::~BlockStream()
{
	listRelease(mBlockList);
	mBlockList = NULL;
}

bool BlockStream::write(void* bytes, int32 count)
{
	int32 writeableCount = getWriteableCount();
	if (writeableCount < count)
	{
		int32 blockCount = count - (mTotalCount - mWriteIndex);
		blockCount = blockCount % mBlockCount == 0 ? blockCount / mBlockCount : blockCount / mBlockCount + 1;
		mTotalCount = (mBlockList->count + blockCount) * mBlockCount;

		for (int32 i = 0; i < blockCount; ++i)
		{
			listAddNodeTail(mBlockList, new int8[mBlockCount]);
		}
	}
	int32 idx = mWriteIndex / mBlockCount;
	int32 idxCount = mWriteIndex % mBlockCount;
	ListNode* listNode = listGetNode(mBlockList, idx);
	if (listNode == NULL) return false;
	if (mBlockCount - idxCount >= count)
	{
		memcpy_s((int8*)listNode->value + idxCount, mBlockCount - idxCount, bytes, count);
		mWriteIndex += count;
		return true;
	}
	int32 crrentCount = 0;
	while (listNode)
	{
		if (mBlockCount - idxCount < count - crrentCount)
		{
			memcpy_s((int8*)listNode->value + idxCount, mBlockCount - idxCount, (int8*)bytes + crrentCount, mBlockCount - idxCount);
			mWriteIndex += mBlockCount - idxCount;
			crrentCount += mBlockCount - idxCount;
			idxCount = 0;
		}
		else
		{
			memcpy_s((int8*)listNode->value + idxCount, mBlockCount - idxCount, (int8*)bytes + crrentCount, count - crrentCount);
			mWriteIndex += count - crrentCount;
			crrentCount += count - crrentCount;
			idxCount = 0;
			break;
		}
		listNode = listNode->next;
	}

	return true;
}

bool BlockStream::read(void* bytes, int32 count)
{
	if (getReadableCount() < count)
		return false;

	int32 idx = mReadIndex / mBlockCount;
	int32 idxCount = mReadIndex % mBlockCount;
	ListNode* listNode = listGetNode(mBlockList, idx);
	if (listNode == NULL) return false;
	if (mBlockCount - idxCount >= count)
	{
		memcpy_s(bytes, count, (int8*)listNode->value + idxCount, count);
		mReadIndex += count;
		return true;
	}

	int32 crrentCount = 0;
	while (listNode)
	{
		if (mBlockCount - idxCount < count - crrentCount)
		{
			memcpy_s((int8*)bytes + crrentCount, count - crrentCount, (int8*)listNode->value + idxCount, mBlockCount - idxCount);
			mReadIndex += mBlockCount - idxCount;
			crrentCount += mBlockCount - idxCount;
			idxCount = 0;
		}
		else
		{
			memcpy_s((int8*)bytes + crrentCount, count - crrentCount, (int8*)listNode->value + idxCount, count - crrentCount);
			mReadIndex += count - crrentCount;
			crrentCount += count - crrentCount;
			idxCount = 0;
			break;
		}

		listNode = listNode->next;
	}

	return true;
}

int32 BlockStream::getReadableCount()
{
	return mTotalCount - mReadIndex;
}

int32 BlockStream::getWriteableCount()
{
	return mTotalCount - mWriteIndex;
}

void BlockStream::Clear()
{
	mWriteIndex = 0;
	mReadIndex = 0;
}

bool BlockStream::operator >> (float32& value)
{
	if (!read(&value, sizeof(float32)))
		return false;

	float32* v = (float32*)&value;
	*v = Shared::ntohf(*v);
	return true;
}

bool BlockStream::operator >> (float64& value)
{
	if (!read(&value, sizeof(float64)))
		return false;

	float64* v = (float64*)&value;
	*v = Shared::ntohd(*v);

	return true;
}

bool BlockStream::operator >> (std::string& value)
{
	int16 size = 0;
	if (!(*this >> size))
		return false;

	if (size <= 0)
		return true;

	int8* str = (int8*)malloc(size + 1);
	if (!read(str, size))
	{
		free(str);
		return false;
	}

	str[size] = 0;
	value = str;
	free(str);
	return true;
}

bool BlockStream::operator >> (int8& value)
{
	return read(&value, sizeof(int8));
}

bool BlockStream::operator >> (uint8& value)
{
	return read(&value, sizeof(uint8));
}

bool BlockStream::operator >> (int16& value)
{
	if (!read(&value, sizeof(int16)))
		return false;

	int16* v = (int16*)&value;
	*v = Shared::ntohs(*v);
	return true;
}

bool BlockStream::operator >> (uint16& value)
{
	if (!read(&value, sizeof(uint16)))
		return false;

	uint16* v = (uint16*)&value;
	*v = Shared::ntohs(*v);

	return true;
}

bool BlockStream::operator >> (int32& value)
{
	if (!read(&value, sizeof(int32)))
		return false;

	int32* v = (int32*)&value;
	*v = Shared::ntohl(*v);

	return true;
}

bool BlockStream::operator >> (uint32& value)
{
	if (!read(&value, sizeof(uint32)))
		return false;

	uint32* v = (uint32*)&value;
	*v = Shared::ntohl(*v);

	return true;
}

bool BlockStream::operator >> (int64& value)
{
	if (!read(&value, sizeof(int64)))
		return false;

	int64* v = (int64*)&value;
	*v = Shared::ntohll(*v);

	return true;
}

bool BlockStream::operator >> (uint64& value)
{
	if (!read(&value, sizeof(uint64)))
		return false;

	uint64* v = (uint64*)&value;
	*v = Shared::ntohll(*v);

	return true;
}

bool BlockStream::operator >> (Variant& value)
{
	int8 type_ = 0;
	CHECK(*this >> type_);
	switch (type_)
	{

	case Variant::TypeBoolean:
	{
		int8 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeInt8:
	{
		int8 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeUint8:
	{
		uint8 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeInt16:
	{
		int16 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeUint16:
	{
		uint16 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeInt32:
	{
		int32 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeUint32:
	{
		uint32 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeInt64:
	{
		int64 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeUint64:
	{
		uint64 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeFloat32:
	{
		float32 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeFloat64:
	{
		float64 v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	case Variant::TypeString:
	{
		std::string v = 0;
		CHECK(*this >> v);
		value = v;
		return true;
	}
	}
	return false;
}

bool BlockStream::operator >> (Object& value)
{
	return true;
}

bool BlockStream::operator >> (BinaryStream& value)
{
	return true;
}

bool BlockStream::operator<<(BinaryStream& value)
{
	return true;
}

bool BlockStream::operator<<(const Object& value)
{
	return true;
}

bool BlockStream::operator<<(const Variant& value)
{
	CHECK(*this << value.type());

	switch (value.type())
	{
	case Variant::TypeBoolean:
		CHECK(*this << value.valueBool());
		break;
	case Variant::TypeInt8:
		CHECK(*this << value.valueInt8());
		break;
	case Variant::TypeUint8:
		CHECK(*this << value.valueUint8());
		break;
	case Variant::TypeInt16:
		CHECK(*this << value.valueInt16());
		break;
	case Variant::TypeUint16:
		CHECK(*this << value.valueUint16());
		break;
	case Variant::TypeInt32:
		CHECK(*this << value.valueInt32());
		break;
	case Variant::TypeUint32:
		CHECK(*this << value.valueUint32());
		break;
	case Variant::TypeInt64:
		CHECK(*this << value.valueInt64());
		break;
	case Variant::TypeUint64:
		CHECK(*this << value.valueUint64());
		break;
	case Variant::TypeFloat32:
		CHECK(*this << value.valueFloat32());
		break;
	case Variant::TypeFloat64:
		CHECK(*this << value.valueFloat64());
		break;
	case Variant::TypeString:
		CHECK(*this << std::string(value.valueString()));
		break;
	}
	return true;
}

bool BlockStream::operator<<(const uint64 value)
{
	uint64 v = Shared::htonll(value);

	return write(&v, sizeof(value));
}

bool BlockStream::operator<<(const int64 value)
{
	int64 v = Shared::htonll(value);

	return write(&v, sizeof(value));
}

bool BlockStream::operator<<(const uint32 value)
{
	uint32 v = Shared::htonl(value);

	return write(&v, sizeof(value));
}

bool BlockStream::operator<<(const int32 value)
{
	int32 v = Shared::htonl(value);

	return write(&v, sizeof(value));
}

bool BlockStream::operator<<(const uint16 value)
{
	uint16 v = Shared::htons(value);

	return write(&v, sizeof(value));
}

bool BlockStream::operator<<(const int16 value)
{
	int16 v = Shared::htons(value);

	return write(&v, sizeof(value));
}

bool BlockStream::operator<<(const uint8 value)
{
	return write((void*)&value, sizeof(value));
}

bool BlockStream::operator<<(const int8 value)
{
	return write((void*)&value, sizeof(value));
}

bool BlockStream::operator<<(const std::string& value)
{
	int16 size = (int16)value.length();
	*this << size;
	if (size > 0)
		return write((void*)value.c_str(), size);

	return true;
}

bool BlockStream::operator<<(const float64& value)
{
	float64 sv = Shared::htond(value);
	return write(&sv, sizeof(value));
}

bool BlockStream::operator<<(const float32& value)
{
	float sv = Shared::htonf(value);
	return write(&sv, sizeof(value));
}


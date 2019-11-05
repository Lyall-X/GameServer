#include "Shared.hpp"


BinaryStream::BinaryStream(int32 count /* = BINARY_LENGTH */):
mCount(count),
mBytes(NULL),
mWritePosition(0),
mReadPosition(0),
mIsDel(false)
{
	mIsDel = true;
	if (mCount <= 0)
		mBytes = NULL;
	else {
		mBytes = new int8[mCount];
		memset(mBytes, 0, mCount);
	}
}

BinaryStream::BinaryStream(void* ptr, int32 count, bool isDel):
mCount(count),
mBytes((int8*)ptr),
mWritePosition(0),
mReadPosition(0),
mIsDel(isDel)
{

}

BinaryStream::BinaryStream( const BinaryStream& other ):
mCount(other.mCount),
mWritePosition(other.mWritePosition),
mReadPosition(other.mReadPosition)
{
	mIsDel = true;
	mBytes = new int8[mCount];
	memcpy(mBytes, other.mBytes, mCount);
}

BinaryStream::~BinaryStream(void)
{
	release();
}

BinaryStream& BinaryStream::operator=( const BinaryStream& other )
{
	release();
	mCount = other.mCount;
	mWritePosition = other.mWritePosition;
	mReadPosition = other.mReadPosition;

	mIsDel = true;
	mBytes = new int8[mCount];
	memcpy(mBytes, other.mBytes, mCount);
	return *this;
}

void BinaryStream::clear()
{
	mWritePosition = 0;
	mReadPosition = 0;
}

std::string BinaryStream::toString()
{
	return Shared::ConvertHex((char*)datas(), wpos());
}

bool BinaryStream::write(const void* data, int32 count)
{
	if (count <= 0)
		return false;
	if (mWritePosition + count > mCount) {
		resize(mWritePosition + count);
	}

	memcpy(mBytes + mWritePosition, data, count);
	mWritePosition += count;

	return true;
}

bool BinaryStream::push(int32 pos, const void* data, int32 count)
{
	if (count <= 0)
		return false;

	if (pos + count > mCount)
		return false;
	memcpy(mBytes + pos, data, count);
	return true;		
}

bool BinaryStream::read(void* data, int32 count)
{
	if (count <= 0)
		return false;

	if (mReadPosition + count > mCount)
		return false;

	memcpy(data, mBytes + mReadPosition, count);
	mReadPosition += count;
	return true;
}

bool BinaryStream::pop(int32 pos, void* data, int32 count)
{
	if (count <= 0)
		return false;

	if (pos + count > mCount)
		return false;
	memcpy(data, mBytes + pos, count);
	return true;
}

void BinaryStream::resize(int32 size)
{
	if (size <= 0)
		return;
	mCount = size;
	int8* data = new int8[mCount];
	memcpy(data, mBytes, wpos());
	release();
	mIsDel = true;
	mBytes = data;
}

void BinaryStream::release()
{
	if (mIsDel)
		SAFE_DELETE_ARRAY(mBytes);
	mBytes = NULL;
}

bool BinaryStream::operator>>( Variant& value )
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
			std::string v = "";
			CHECK(*this >> v);
			value = v;
			return true;
		}
	case Variant::TypeMemory:
		{
			BinaryStream& bytes = (BinaryStream&)value.valueBytes();
			CHECK(*this >> bytes);
			return true;
		}
	}
	return false;
}

bool BinaryStream::operator<<(const float32& value)
{
	float sv = Shared::htonf(value);

	return write(&sv, sizeof(value));
}

bool BinaryStream::operator<<( const float64& value )
{
	float64 sv = Shared::htond(value);

	return write(&sv, sizeof(value));
}

bool BinaryStream::operator<<(const std::string& value)
{
	int16 size = (int16)value.length();
	*this << size;
	if (size > 0)
		return write(value.c_str(), size);

	return true;
}

bool BinaryStream::operator<<(const char* value)
{
	int16 size = (int16)strlen(value);
	*this << size;
	if (size > 0)
		return write(value, size);

	return true;
}

bool BinaryStream::operator <<(const int8 value)
{
	return write(&value, sizeof(value));
}

bool BinaryStream::operator<<( const Variant& value )
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
	case Variant::TypeMemory:
		BinaryStream& bytes = (BinaryStream&)((Variant&)(value)).valueBytes();
		CHECK(*this << bytes);
		break;
	}
	return true;
}

bool BinaryStream::operator<<(const uint8 value)
{
	return write(&value, sizeof(value));
}

bool BinaryStream::operator<<(const int16 value)
{
	int16 v = Shared::htons(value);
	
	return write(&v, sizeof(value));
}

bool BinaryStream::operator<<(const uint16 value)
{
	uint16 v = Shared::htons(value);
	
	return write(&v, sizeof(value));
}

bool BinaryStream::operator<<(const int32 value)
{
	int32 v = Shared::htonl(value);
	
	return write(&v, sizeof(value));
}

bool BinaryStream::operator<<(const uint32 value)
{
	uint32 v = Shared::htonl(value);
	
	return write(&v, sizeof(value));
}

bool BinaryStream::operator<<(const int64 value)
{
	int64 v = Shared::htonll(value);

	return write(&v, sizeof(value));
}

bool BinaryStream::operator<<(const uint64 value)
{
	uint64 v = Shared::htonll(value);
	
	return write(&v, sizeof(value));
}

bool BinaryStream::operator<<(const Object& value)
{
	Object& obj = (Object&)value;
	return obj >> (*this);
}

bool BinaryStream::operator<<(BinaryStream& value)
{
	int32 count = value.wpos();
	if (!(*this << count))
		return false;

	if (count <= 0)
		return true;

	return write(value.datas(), value.wpos());
}

bool BinaryStream::operator>>( float32& value )
{
	if (!read(&value, sizeof(float32)))
		return false;
	
	float32* v = (float32*)&value;
	*v = Shared::ntohf(*v);
	return true;
}

bool BinaryStream::operator>>(float64& value )
{
	if (!read(&value, sizeof(float64)))
		return false;

	float64* v = (float64*)&value;
	*v = Shared::ntohd(*v);

	return true;
}

bool BinaryStream::operator>>(std::string& value)
{
	int16 size = 0;
	if (!(*this >> size))
		return false;

	if (size <= 0)
		return true;

	char* str = new char[size + 1];
	if (!read(str, size))
	{
		delete[] str;
		return false;
	}

	str[size] = 0;
	value = str;
	delete[] str;
	return true;
}

bool BinaryStream::operator>>(int8& value )
{
	return read(&value, sizeof(int8));
}

bool BinaryStream::operator>>(uint8& value )
{
	return read(&value, sizeof(uint8));
}

bool BinaryStream::operator>>(int16& value )
{
	if (!read(&value, sizeof(int16)))
		return false;
	
	int16* v = (int16*)&value;
	*v = Shared::ntohs(*v);
	return true;
}

bool BinaryStream::operator>>(uint16& value )
{
	if (!read(&value, sizeof(uint16)))
		return false;

	uint16* v = (uint16*)&value;
	*v = Shared::ntohs(*v);

	return true;
}

bool BinaryStream::operator>>( int32& value )
{
	if (!read(&value, sizeof(int32)))
		return false;

	int32* v = (int32*)&value;
	*v = Shared::ntohl(*v);

	return true;
}

bool BinaryStream::operator>>(uint32& value )
{
	if (!read(&value, sizeof(uint32)))
		return false;

	uint32* v = (uint32*)&value;
	*v = Shared::ntohl(*v);

	return true;
}

bool BinaryStream::operator>>(int64& value )
{
	if (!read(&value, sizeof(int64)))
		return false;

	int64* v = (int64*)&value;
	*v = Shared::ntohll(*v);

	return true;
}

bool BinaryStream::operator>>( uint64& value )
{
	if (!read(&value, sizeof(uint64)))
		return false;

	uint64* v = (uint64*)&value;
	*v = Shared::ntohll(*v);

	return true;
}

bool BinaryStream::operator>>(Object& value)
{
	return value << (*this);
}

bool BinaryStream::operator>>(BinaryStream& value)
{
	int bit_size = 0;
	if (!(*this >> bit_size))
		return false;

	if (bit_size <= 0)
		return true;
	
	int8* bit_data = new int8[bit_size];
	if (!read(bit_data, bit_size))
	{
		delete [] bit_data;
		return false;
	}

	if (!value.write(bit_data, bit_size))
	{
		delete [] bit_data;
		return false;
	}

	delete [] bit_data;
	return true;	 
}

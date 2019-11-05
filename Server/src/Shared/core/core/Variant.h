#pragma once

class BinaryStream;

#pragma pack(push)
#pragma pack(1)

struct object_str
{
	uint32		len;
	const char* str;
};

struct object_bytes
{
	uint32				len;
	const char*			ptr;
};

union object_union
{
	bool				value_bool;
	int8				value_char;
	uint8				value_uchar;
	int16				value_short;
	uint16				value_ushort;
	int32				value_int;
	uint32				value_uint;
	int64				value_int64;
	uint64				value_uint64;
	float32				value_float;
	float64				value_double;
	char*				value_str;
	void*				value_pointer;
	BinaryStream*		value_bytes;
};


class Variant
{
	friend class Dictionary;
public:
	enum
	{
		TypeNone,
		TypeBoolean,
		TypeInt8,
		TypeUint8,
		TypeInt16,
		TypeUint16,
		TypeInt32,
		TypeUint32,
		TypeInt64,
		TypeUint64,
		TypeFloat32,
		TypeFloat64,
		TypeString,
		TypeMemory,
		TypePointer,
		TypeDate,
	};
public:
	static char convert(const char* str);
	Variant();
	~Variant(void);
	Variant(const Variant& other);
	Variant(const bool value);
	Variant(const int8 value);
	Variant(const uint8 value);
	Variant(const int16 value);
	Variant(const uint16 value);
	Variant(const int32 value);
	Variant(const uint32 value);
	Variant(const int64 value);
	Variant(const uint64 value);
	Variant(const float32 value);
	Variant(const float64 value);
	Variant(const std::string& value);
	Variant(const char* value);
	Variant(const void* value);
	Variant(const BinaryStream& value);
	Variant(int8* value, int32 length);
public:
	Variant& operator= (const Variant& value);
	Variant& operator= (const bool value);
	Variant& operator= (const int8 value);
	Variant& operator= (const uint8 value);
	Variant& operator= (const int16 value);
	Variant& operator= (const uint16 value);
	Variant& operator= (const int32 value);
	Variant& operator= (const uint32 value);
	Variant& operator= (const int64 value);
	Variant& operator= (const uint64 value);
	Variant& operator= (const float32 value);
	Variant& operator= (const float64 value);
	Variant& operator= (const std::string& value);
	Variant& operator= (const void* value);
	Variant& operator= (const BinaryStream& value);
public:
	bool operator > (const Variant& other) const;
	bool operator >= (const Variant& other) const;
	bool operator < (const Variant& other) const;
	bool operator <= (const Variant& other) const;
	bool operator == (const Variant& other) const;
	bool operator != (const Variant& other)  const;
	operator int8();
	operator uint8();
	operator int16();
	operator uint16();
	operator int32();
	operator uint32();
	operator int64();
	operator uint64();
	operator float32();
	operator float64();
	operator char*();
	operator const char*();
	operator BinaryStream&();
public:
	const char type() const { return mType; }
	const std::string toString() const;
	const bool valueBool() const;
	const int8 valueInt8() const;
	const uint8 valueUint8() const;
	const int16 valueInt16() const;
	const uint16 valueUint16() const;
	const int32 valueInt32() const;
	const uint32 valueUint32() const;
	const int64 valueInt64() const;
	const uint64 valueUint64() const;
	const float32 valueFloat32() const;
	const float64 valueFloat64() const;
	const char* valueString() const;
	const void* valuePointer() const;
	const BinaryStream& valueBytes();
protected:
	void reset();
protected:

	object_union			mValue;
	char					mType;
protected:
	void copyValue(const object_union& src, object_union& dst, int8 type);

};

#pragma pack(pop)
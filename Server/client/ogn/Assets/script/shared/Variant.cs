using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;




public class Variant
{
    public const int TypeNone = 0;
    public const int TypeBoolean = 1;
    public const int TypeInt8 = 2;
    public const int TypeUint8 = 3;
    public const int TypeInt16 = 4;
    public const int TypeUint16 = 5;
    public const int TypeInt32 = 6;
    public const int TypeUint32 = 7;
    public const int TypeInt64 = 8;
    public const int TypeUint64 = 9;
    public const int TypeFloat32 = 10;
    public const int TypeFloat64 = 11;
    public const int TypeString = 12;
    public const int TypePointer = 13;
    public const int TypeDate = 14;
    public const int TypeMemory = 15;


    public static int convert(string str)
    {
        if (str == "bool")
            return TypeBoolean;
        else if (str == "int8")
            return TypeInt8;
        else if (str == "uint8")
            return TypeUint8;
        else if (str == "int16")
            return TypeInt16;
        else if (str == "uint16")
            return TypeUint16;
        else if (str == "int32")
            return TypeInt32;
        else if (str == "uint32")
            return TypeUint32;
        else if (str == "int64")
            return TypeInt64;
        else if (str == "uint64")
            return TypeUint64;
        else if (str == "float32")
            return TypeFloat32;
        else if (str == "float64")
            return TypeFloat64;
        else if (str == "string")
            return TypeString;
        return TypeNone;
    }
    public struct Value
    {
        public bool value_bool;
        public byte value_int8;
        public sbyte value_uint8;
        public short value_int16;
        public ushort value_uint16;
        public int value_int32;
        public uint value_uint32;
        public long value_int64;
        public ulong value_uint64;
        public float value_float32;
        public double value_float64;
        public string value_string;
    }

    public byte mType = 0;

    public Value mValue = new Value();

    public Variant()
    {
    }

    public Variant(Variant other)
    {
        Copy(other);
    }

    public Variant(bool value)
    {
        mType = TypeBoolean;
        mValue.value_bool = value;
    }

    public Variant(byte value)
    {
        mType = TypeInt8;
        mValue.value_int8 = value;
    }

    public Variant(sbyte value)
    {
        mType = TypeUint8;
        mValue.value_uint8 = value;
    }

    public Variant(short value)
    {
        mType = TypeInt16;
        mValue.value_int16 = value;
    }

    public Variant(ushort value)
    {
        mType = TypeUint16;
        mValue.value_uint16 = value;
    }

    public Variant(int value)
    {
        mType = TypeInt32;
        mValue.value_int32 = value;
    }

    public Variant(uint value)
    {
        mType = TypeUint32;
        mValue.value_uint32 = value;
    }

    public Variant(long value)
    {
        mType = TypeInt64;
        mValue.value_int64 = value;
    }

    public Variant(ulong value)
    {
        mType = TypeUint64;
        mValue.value_uint64 = value;
    }

    public Variant(float value)
    {
        mType = TypeFloat32;
        mValue.value_float32 = value;
    }

    public Variant(double value)
    {
        mType = TypeFloat64;
        mValue.value_float64 = value;
    }

    public Variant(string value)
    {
        mType = TypeString;
        mValue.value_string = value;
    }

    public bool GetBool()
    {
        if (mType == TypeBoolean)
            return mValue.value_bool;
        return false;
    }

    public byte GetInt8()
    {
        if (mType == TypeInt8)
            return mValue.value_int8;
        return 0;
    }

    public sbyte GetUint8()
    {
        if (mType == TypeUint8)
            return mValue.value_uint8;
        return 0;
    }

    public short GetInt16()
    {
        if (mType == TypeInt16)
            return mValue.value_int16;
        return 0;
    }
    public ushort GetUint16()
    {
        if (mType == TypeUint16)
            return mValue.value_uint16;
        return 0;
    }
    public int GetInt32()
    {
        if (mType == TypeInt32)
            return mValue.value_int32;
        return 0;
    }
    public uint GetUint32()
    {
        if (mType == TypeUint32)
            return mValue.value_uint32;
        return 0;
    }
    public long GetInt64()
    {
        if (mType == TypeInt64)
            return mValue.value_int64;
        return 0;
    }
    public ulong GetUint64()
    {
        if (mType == TypeBoolean)
            return mValue.value_uint64;
        return 0;
    }
    public float GetFloat32()
    {
        if (mType == TypeFloat32)
            return mValue.value_float32;
        return 0.0f;
    }
    public double GetFloat64()
    {
        if (mType == TypeFloat64)
            return mValue.value_float64;
        return 0.0;
    }
    public string GetString()
    {
        if (mType == TypeString)
            return mValue.value_string;
        return "";
    }

    public void SetBool(bool value)
    {
        mType = TypeBoolean;
        mValue.value_bool = value;
    }

    public void SetInt8(byte value)
    {
        mType = TypeInt8;
        mValue.value_int8 = value;
    }

    public void SetUint8(sbyte value)
    {
        mType = TypeUint8;
        mValue.value_uint8 = value;
    }

    public void SetInt16(short value)
    {
        mType = TypeInt16;
        mValue.value_int16 = value;
    }

    public void SetUint16(ushort value)
    {
        mType = TypeUint16;
        mValue.value_uint16 = value;
    }

    public void SetInt32(int value)
    {
        mType = TypeInt32;
        mValue.value_int32 = value;
    }

    public void SetUint32(uint value)
    {
        mType = TypeUint32;
        mValue.value_uint32 = value;
    }

    public void SetInt64(Int64 value)
    {
        mType = TypeInt64;
        mValue.value_int64 = value;
    }

    public void SetUint64(UInt64 value)
    {
        mType = TypeUint64;
        mValue.value_uint64 = value;
    }

    public void SetFloat32(float value)
    {
        mType = TypeFloat32;
        mValue.value_float32 = value;
    }

    public void SetFloat64(double value)
    {
        mType = TypeFloat64;
        mValue.value_float64 = value;
    }

    public void SetString(string value)
    {
        mType = TypeString;
        mValue.value_string = value;
    }

    public void Copy(Variant other)
    {
        mType = other.mType;
        switch (mType)
        {
            case Variant.TypeBoolean:
                mValue.value_bool = other.mValue.value_bool;
                break;
            case Variant.TypeInt8:
                mValue.value_uint8 = other.mValue.value_uint8;
                break;
            case Variant.TypeUint8:
                mValue.value_bool = other.mValue.value_bool;
                break;
            case Variant.TypeInt16:
                mValue.value_int16 = other.mValue.value_int16;
                break;
            case Variant.TypeUint16:
                mValue.value_uint16 = other.mValue.value_uint16;
                break;
            case Variant.TypeInt32:
                mValue.value_int32 = other.mValue.value_int32;
                break;
            case Variant.TypeUint32:
                mValue.value_uint32 = other.mValue.value_uint32;
                break;
            case Variant.TypeInt64:
                mValue.value_int64 = other.mValue.value_int64;
                break;
            case Variant.TypeUint64:
                mValue.value_uint64 = other.mValue.value_uint64;
                break;
            case Variant.TypeFloat32:
                mValue.value_float32 = other.mValue.value_float32;
                break;
            case Variant.TypeFloat64:
                mValue.value_float64 = other.mValue.value_float64;
                break;
            case Variant.TypeString:
                mValue.value_string = other.mValue.value_string;
                break;
        }
    }

    public static bool operator ==(Variant v0, Variant v1)
    {
        if (v0.mType != v1.mType)
            return false;
        switch (v0.mType)
        {
            case Variant.TypeBoolean:
                return v0.mValue.value_bool == v1.mValue.value_bool;
            case Variant.TypeInt8:
                return v0.mValue.value_uint8 == v1.mValue.value_uint8;
            case Variant.TypeUint8:
                return v0.mValue.value_bool == v1.mValue.value_bool;
            case Variant.TypeInt16:
                return v0.mValue.value_int16 == v1.mValue.value_int16;
            case Variant.TypeUint16:
                return v0.mValue.value_uint16 == v1.mValue.value_uint16;
            case Variant.TypeInt32:
                return v0.mValue.value_int32 == v1.mValue.value_int32;
            case Variant.TypeUint32:
                return v0.mValue.value_uint32 == v1.mValue.value_uint32;
            case Variant.TypeInt64:
                return v0.mValue.value_int64 == v1.mValue.value_int64;
            case Variant.TypeUint64:
                return v0.mValue.value_uint64 == v1.mValue.value_uint64;
            case Variant.TypeFloat32:
                return v0.mValue.value_float32 == v1.mValue.value_float32;
            case Variant.TypeFloat64:
                return v0.mValue.value_float64 == v1.mValue.value_float64;
            case Variant.TypeString:
                return v0.mValue.value_string == v1.mValue.value_string;
        }
        return false;
    }

    public static bool operator !=(Variant v0, Variant v1)
    {
        if (v0 == v1)
            return false;
        return true;
    }

}

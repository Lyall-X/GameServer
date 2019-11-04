using System.Collections;
using System.IO;
using System;
using System.Text;

public class BinaryStream 
{
    public const int PACKET_MAX_LENGTH = 4096;
    public static bool HostEqualNet()
    {
        short v = 1;
        byte[] data = BitConverter.GetBytes(v);
        if (data[0] == 1) return false;
        return true;             
    }

    public static int ntohl(int value)
    {
        if (HostEqualNet())
            return value;

        byte[] data = NetToHost(BitConverter.GetBytes(value));

        return BitConverter.ToInt32(data, 0);
    }

    //将主机顺序转换为网络顺序;
    public static byte[] HostToNet(byte[] data)
    {
        if (HostEqualNet())
            return data;

        byte[] temp = new byte[data.Length];
        for (int i = 0; i < data.Length; ++i )
            temp[i] = data[data.Length - i - 1];

        return temp;
    }

    //将网络顺序转换为主机顺序;
    public static byte[] NetToHost(byte[] data)
    {
        byte[] temp = new byte[data.Length];
        for (int i = 0; i < data.Length; ++i)
            temp[i] = data[data.Length - i - 1];

        return temp;
    }

    public BinaryStream ()
    {
        mBuffer = new byte[Shared.MAX_PACKET_LENGTH];
        mWriteIndex = 0;
        mReadIndex = 0;
    }

    public BinaryStream(int count)
    {
        mBuffer = new byte[count];
        mWriteIndex = 0;
        mReadIndex = 0;
    }

    public BinaryStream(byte[] data)
    {
        mBuffer = data;
        mWriteIndex = 0;
        mReadIndex = 0;
    }
    public BinaryStream(byte[] data, int offset, int length)
    {
        mBuffer = new byte[length];
        mWriteIndex = 0;
        mReadIndex = 0;

        Write(data, offset, length);
    }

    public bool Write(float value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(double value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(string value)
    {
        ushort leng = 0;
        byte[] data = null;
        if (value.Length > 0)
        {
            data = Encoding.UTF8.GetBytes(value);
            leng = (ushort)data.Length;
        }

        byte[] leng_data = HostToNet(BitConverter.GetBytes(leng));
        if (!Write(leng_data))
            return false;

        if (leng > 0)
            return Write(data);
       
        return true;
    }

    public bool Write(bool value)
    {
        validBuffer(1);
        mBuffer[mWriteIndex] = value ? (byte)1 : (byte)0;
        mWriteIndex += sizeof(byte);
        return true;
    }

    public bool Write(byte value)
    {
        validBuffer(1);
        mBuffer[mWriteIndex] = value;
        mWriteIndex += sizeof(byte);
        return true;
    }

    public bool Write(sbyte value)
    {
        validBuffer(1);
        mBuffer[mWriteIndex] = (byte)value;
        mWriteIndex += sizeof(sbyte);
        return true;
    }

    public bool Write(char value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(short value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(ushort value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(int value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(uint value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(long value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }

    public bool Write(ulong value)
    {
        byte[] data = HostToNet(BitConverter.GetBytes(value));
        if (!Write(data))
            return false;
        return true;
    }
    public bool Write(BinaryStream value)
    {
        if (!this.Write(value.wpos))
            return false;
        
        if (value.wpos <= 0)
            return true;
         
        if (!this.Write(value.buffer, 0, value.wpos))
            return false;
        
        return true;
    }

    public bool Write(Variant value)
    {
        Write(value.mType);
        switch (value.mType)
        {
            case Variant.TypeBoolean:
                return Write(value.GetBool());
            case Variant.TypeInt8:
                return Write(value.GetInt8());
            case Variant.TypeUint8:
                return Write(value.GetUint8());
            case Variant.TypeInt16:
                return Write(value.GetInt16());
            case Variant.TypeUint16:
                return Write(value.GetUint16());
            case Variant.TypeInt32:
                return Write(value.GetInt32());
            case Variant.TypeUint32:
                return Write(value.GetUint32());
            case Variant.TypeInt64:
                return Write(value.GetInt64());
            case Variant.TypeUint64:
                return Write(value.GetUint64());
            case Variant.TypeFloat32:
                return Write(value.GetFloat32());
            case Variant.TypeFloat64:
                return Write(value.GetFloat64());
            case Variant.TypeString:
                return Write(value.GetString());
        }
        return true;
    }

    public bool Write(byte[] buffer, int index = 0, int count = -1)
    {
        count = count == -1 ? buffer.Length : count;
        if (!validBuffer(count))
            return false;
        Array.Copy(buffer, index, mBuffer, mWriteIndex, count);
        mWriteIndex += count;
        return true;
    }

    public bool validBuffer(int count)
    {
        if (mBuffer.Length - mWriteIndex < count)
        {
            byte[] datas = new byte[mBuffer.Length + count];
            Array.Copy(mBuffer, 0, datas, 0, mBuffer.Length);
            mBuffer = datas;
            //Console.WriteLine("空间不足!");
            return true;
        }
        return true;
    }

    public bool validData(int count)
    {
        if (mBuffer.Length - mReadIndex < count)
        {
            Console.WriteLine("空间不足!");
            return false;
        }
        return true;
    }
 

    public bool Write(Header value)
    {
        if (!value.serialize(this))
            return false;
        return true;
    }

    public bool Read(ref float value)
    {
        int length = sizeof(float);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;
        value = BitConverter.ToSingle(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref double value)
    {
        int length = sizeof(double);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;
        value = BitConverter.ToDouble(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref string value)
    {
        int length = sizeof(short);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        short str_len = BitConverter.ToInt16(NetToHost(data), 0);

        if (str_len > 0)
        {
            byte[] str_data = new byte[str_len];
            if (!ReadBytes(str_data))
                return false;
            value = Encoding.UTF8.GetString(str_data, 0, str_len);
        }
        return true;
    }

    public bool Read(ref bool value)
    {
        if (!validData(sizeof(sbyte)))
            return false;
        value = mBuffer[mReadIndex] == 1 ? true : false;
        mReadIndex += sizeof(byte);
        return true;
    }

    public bool Read(ref byte value)
    {
        if (!validData(sizeof(sbyte)))
            return false;
        value = mBuffer[mReadIndex];
        mReadIndex += sizeof(byte);
        return true;
    }

    public bool Read(ref sbyte value)
    {
        if (!validData(sizeof(sbyte)))
            return false;
        value = (sbyte)mBuffer[mReadIndex];
        mReadIndex += sizeof(sbyte);
        return true;
    }

    public bool Read(ref char value)
    {
        int length = sizeof(char);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToChar(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref short value)
    {
        int length = sizeof(short);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToInt16(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref ushort value)
    {
        int length = sizeof(ushort);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToUInt16(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref int value)
    {
        int length = sizeof(int);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToInt32(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref uint value)
    {
        int length = sizeof(uint);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToUInt32(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref long value)
    {
        int length = sizeof(long);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToInt64(NetToHost(data), 0);
        return true;
    }

    public bool Read(ref ulong value)
    {
        int length = sizeof(ulong);

        byte[] data = new byte[length];
        if (!ReadBytes(data))
            return false;

        value = BitConverter.ToUInt64(NetToHost(data), 0);
        return true;
    }

    public bool ReadBytes(byte[] buffer, int index = 0, int length = -1)
    {
        length = length == -1 ? buffer.Length : length;
        if (!validData(length))
            return false;
        Array.Copy(mBuffer, mReadIndex, buffer, index, length);
        mReadIndex += length;

        return true;
    }


    public bool Read(Header value)
    {
        return value.deSerialize(this);
    }

    public bool Read(Variant value)
    {
        Read(ref value.mType);
        switch (value.mType)
        {
            case Variant.TypeBoolean:
                return Read(ref value.mValue.value_bool);
            case Variant.TypeInt8:
                return Read(ref value.mValue.value_int8);
            case Variant.TypeUint8:
                return Read(ref value.mValue.value_uint8);
            case Variant.TypeInt16:
                return Read(ref value.mValue.value_int16);
            case Variant.TypeUint16:
                return Read(ref value.mValue.value_uint16);
            case Variant.TypeInt32:
                return Read(ref value.mValue.value_int32);
            case Variant.TypeUint32:
                return Read(ref value.mValue.value_uint32);
            case Variant.TypeInt64:
                return Read(ref value.mValue.value_int64);
            case Variant.TypeUint64:
                return Read(ref value.mValue.value_uint64);
            case Variant.TypeFloat32:
                return Read(ref value.mValue.value_float32);
            case Variant.TypeFloat64:
                return Read(ref value.mValue.value_float64);
            case Variant.TypeString:
                return Read(ref value.mValue.value_string);
        }
        return true;
    }

    public bool Read(BinaryStream value)
    {
        int len = 0;
        this.Read(ref len);
        if (len >  0)
        {
            byte[] bytes = new byte[len];
            if (!ReadBytes(bytes))
                return false;
            value.Write(bytes, 0, len);       
        }
        return true;
    }

    public void Clear()
    {
        mWriteIndex = 0;
        mReadIndex = 0;
    }


    public byte[] buffer
    {
        get { return mBuffer; }
    }

    public int wpos
    {
        get { return mWriteIndex;  }
        set { mWriteIndex = value; }
    }

    public int rpos
    {
        get { return mReadIndex; }
        set { mReadIndex = value; }
    }

    byte [] mBuffer;
    int mWriteIndex;
    int mReadIndex;
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

public class CircleBuffer
{
    public const int PACKET_MAX_LENGTH = 4096;
    public CircleBuffer(int count = PACKET_MAX_LENGTH)
    {
        this.mCount = count;
        this.buffer = new byte[count];
        readPosition = 0;
        writePosition = 0;
    }

    public bool Push(byte[] bytes, int offset, int count)
    {
        if (count <= 0) return false;

        if (mCount - validDataLength < count) return false;

        if (writePosition < readPosition)
        {
            Array.Copy(bytes, offset, this.buffer, writePosition, count);

            writePosition += count;
            writePosition = (writePosition >= mCount) ? 0 : writePosition;
            return true;
        }

        if (mCount - writePosition >= count)
        {
            Array.Copy(bytes, offset, this.buffer, writePosition, count);
            writePosition += count;
            writePosition = (writePosition >= mCount) ? 0 : writePosition;
            return true;
        }

        int size = mCount - writePosition;

        Array.Copy(bytes, offset, this.buffer, writePosition, size);
        writePosition = 0;

        Array.Copy(bytes, offset + size, this.buffer, writePosition, count - size);
        writePosition += count - size;
        writePosition = (writePosition >= mCount) ? 0 : writePosition;
        return true;
    }

    public bool Pop(byte[] bytes, int offset, int count)
    {
        if (count <= 0) return false;
        if (bytes.Length - offset < count) return false;
        if (validDataLength < count) return false;

        if (writePosition > readPosition)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            readPosition += count;

            readPosition = (readPosition >= mCount) ? 0 : readPosition;
            if (readPosition == writePosition)
                readPosition = writePosition = 0;

            return true;
        }

        if (mCount - readPosition >= count)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            readPosition += count;
            readPosition = (readPosition >= mCount) ? 0 : readPosition;
            if (readPosition == writePosition)
                readPosition = writePosition = 0;

            return true;
        }

        int size = mCount - readPosition;

        Array.Copy(this.buffer, readPosition, bytes, offset, size);
        readPosition = 0;
        Array.Copy(this.buffer, readPosition, bytes, offset + size, count - size);
        readPosition += count - size;
        readPosition = (readPosition >= mCount) ? 0 : readPosition;
        if (readPosition == writePosition)
            readPosition = writePosition = 0;

        return true;
    }

    public bool Read(byte[] bytes, int offset, int count)
    {
        if (count <= 0) return false;
        if (validDataLength < count) return false;
        if (writePosition > readPosition)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            return true;
        }

        if (mCount - readPosition >= count)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            return true;
        }

        int size = mCount - readPosition;
        Array.Copy(this.buffer, readPosition, bytes, offset, size);
        Array.Copy(this.buffer, 0, bytes, offset + size, count - size);
        return true;
    }

    public void Clear()
    {
        readPosition = 0;
        writePosition = 0;
    }

    public int writeLength
    {
        get 
        {
            if (readPosition > writePosition)
                return readPosition - writePosition;

            if (readPosition < writePosition)
                return mCount - writePosition;

            if (readPosition == 0)
                return mCount;

            return 0;
        }
    }

    public int validDataLength
    {
        get {
            if (readPosition == writePosition)
                return 0;

            if (readPosition > writePosition)
                return mCount - readPosition + writePosition;

            return writePosition - readPosition;
        }
    }

    public byte[] buffer;
    public int mCount;
    public int readPosition;
    public int writePosition;
}


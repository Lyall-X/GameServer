using System.Collections;

using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Collections.Generic;
using System.IO;
using System;
using ComponentAce.Compression.Libs.zlib;

public class SocketEvent : EventTarget
{
	public static string CONNECT = "onConnect";
    public static string ACCEPT = "onAccept";
    public static string RECV = "onRecv";
    public static string SEND = "onSend";
    public static string EXIT = "onExit";
    public static string EXCEPTION = "onException";
    public SocketEntity socket;
    public byte[] data = null;
}

public class Header
{
    public virtual bool serialize(BinaryStream bytes)
    {
        onSerializeHeader(bytes);
        OnSerialize(bytes);
        return true;
    }
    public virtual bool deSerialize(BinaryStream bytes)
    {
        onDeserializeHeader(bytes);
        OnDeserialize(bytes);
        return true;
    }
    protected virtual bool onSerializeHeader(BinaryStream bytes)
    {
        return true;
    }

    protected virtual bool onDeserializeHeader(BinaryStream bytes)
    {
        return true;
    }

    protected virtual bool OnSerialize(BinaryStream bytes)
    {
        return true;
    }

    protected virtual bool OnDeserialize(BinaryStream bytes)
    {
        return true;
    }
}

public class Packet : Header
{
    public int mMsgId;
    public int mVersion;
    public Packet(int msgId)
    {
        mMsgId = msgId;
    }

    public int MsgID
    {
        get { return mMsgId; }
    }

    protected override bool onSerializeHeader(BinaryStream bytes)
    {
        bytes.Write(mMsgId);
        bytes.Write(mVersion);
        return true;
    }

    protected override bool onDeserializeHeader(BinaryStream bytes)
    {
        bytes.Read(ref mMsgId);
        bytes.Read(ref mVersion);
        return true;
    }
}

public class PacketHelper
{

    public static PacketHelper instance = new PacketHelper();
    public Object register = null;
    public PacketHelper()
    {
        m_dict = new Dictionary<int, string>();
        m_dict_helper = new Dictionary<int, PacketHandler>();
    }

    public void RegisterPacket(int MsgId, string type_name)
    {
        m_dict.Add(MsgId, type_name);
        string handler_name = type_name + "Handler";
        Type type_ = Type.GetType(handler_name);
        if (type_ != null)
        {
            object obj = type_.Assembly.CreateInstance(handler_name);
            if (obj != null)
            {
                PacketHandler helper = obj as PacketHandler;
                if (helper != null)
                    m_dict_helper.Add(MsgId, helper);
            }
        }
    }

    public Packet AllocPacket(int MsgId)
    {
        if (register == null)
        {
            Type type_ = Type.GetType("PacketRegister");
            if (type_ != null)
                register = type_.Assembly.CreateInstance("PacketRegister");
        }

        string type_name = null;
        if (m_dict.ContainsKey(MsgId))
            type_name = m_dict[MsgId];

        if (type_name != null)
        {
            object obj = Type.GetType(type_name).Assembly.CreateInstance(type_name);
            if (obj != null)
                return obj as Packet;
        }

        return null;
    }

    public PacketHandler FindPacket(int MsgId)
    {
        if (m_dict_helper.ContainsKey(MsgId))
            return m_dict_helper[MsgId];

        return null;
    }

    Dictionary<int, string> m_dict;
    Dictionary<int, PacketHandler> m_dict_helper;
}

public class PacketHandler
{
    public virtual void OnHandler(Packet packet) {}
}

public class SocketEntity
{
    public Socket socket = null;
    public SocketAngent angent;

    public int offset = 0;
    public int packet = 0;
    public int position = 0;
    public byte[] recvPacket = new byte[4096];
    public byte[] countBytes = new byte[4];
    public IPEndPoint ipPoint;
    public Queue<BinaryStream> sendQueue = new Queue<BinaryStream>();
    public CircleBuffer buffer = new CircleBuffer();
    public bool sending = false;

    public uint socketId
    {
        get { return (uint)socket.Handle; }
    }

    public void SendPacket(Packet packet)
    {
        byte[] send_data = new byte[4096];

        BinaryStream bit = new BinaryStream(send_data);
        packet.serialize(bit);
        int length = bit.wpos + 4;
        byte[] send_packet = new byte[length];

        this.SendBuffer(bit.buffer, bit.wpos);
    }

    public void SendBuffer(byte[] data)
    {
        this.SendBuffer(data, data.Length);
    }
    public void SendBuffer(BinaryStream bytes)
    {
        SendBuffer(bytes.buffer, bytes.wpos);
    }

    public void SendBuffer(byte[] data, int length, int index = 0)
    {
        BinaryStream send_bit = new BinaryStream(length + 4);
        send_bit.Write(length + 4);
        send_bit.Write(data, 0, length);

        sendQueue.Enqueue(send_bit);

        postSend();
    }
    protected void OnSend(IAsyncResult ar)
    {
        try
        {
            int BytesSend = socket.EndSend(ar);
            if (BytesSend > 0)
                sending = false;

            postSend();

            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.SEND;
            if (angent.network != null)
                angent.network.dispatchEvent(this, e);
        }
        catch (SocketException ex)
        {
            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.EXCEPTION;
            e.data = System.Text.Encoding.Default.GetBytes(ex.Message);
            if (angent.network != null)
                angent.network.dispatchEvent(this, e);
        }
    }
    public bool postSend()
    {
        if (sending)
            return false;
        if (sendQueue.Count <= 0)
        {
            sending = false;
            return false;
        }
        int sendCount = 0;
        Network.sPacketBuffer.Clear();
        while (true)
        {
            if (sendQueue.Count <= 0)
                break;
            BinaryStream packet = sendQueue.Peek();
            if (sendCount + packet.wpos > BinaryStream.PACKET_MAX_LENGTH)
                break;
            Network.sPacketBuffer.Write(packet.buffer, 0, packet.wpos);
            sendCount += packet.wpos;
            sendQueue.Dequeue();
        }

        try
        {
            sending = true;
            this.socket.BeginSend(Network.sPacketBuffer.buffer, 0, Network.sPacketBuffer.wpos, SocketFlags.None, new AsyncCallback(this.OnSend), this);
        }
        catch
        {
            sending = false;
            return false;
        }
        return true;
    }
}
public class SocketAngent : EventDispatcher
{
    public Network network;
    public SocketEntity socket;
}

public class SocketClient : SocketAngent
{
    public void SendPacket(Packet packet)
    {
        if (socket == null) return;
        socket.SendPacket(packet);
    }
}

public class SocketListener : SocketAngent
{
    public Dictionary<uint, SocketEntity> dictSocket = new Dictionary<uint, SocketEntity>();
    public void SendPacket(SocketEntity socketEnt, Packet packet)
    {
        socketEnt.SendPacket(packet);
    }
}


public class Network : Singleton<Network>
{
    public static BinaryStream sPacketBuffer = new BinaryStream();
    PacketHelper m_helper;
    public delegate void PacketHandler_CallBack(Packet pt);

    protected Queue<SocketEvent> eventList = new Queue<SocketEvent>();
    protected Queue<SocketEntity> clientList = new Queue<SocketEntity>();

    private EventWaitHandle handle = new EventWaitHandle(true, EventResetMode.AutoReset);

    public Network()
    {
        m_helper = new PacketHelper();
        //PacketRegister register = new PacketRegister(m_helper);
    }

    public void update()
    {
        if (eventList.Count > 0)
        {
            handle.WaitOne();
            SocketEvent e = eventList.Dequeue();
            SocketEntity a = clientList.Dequeue();
            SocketAngent angent = a.angent;
            handle.Set();
            angent.dispatchEvent(e);
        }
    }

    public SocketClient Connect(string ip, int port)
    {
        SocketClient client = new SocketClient();

        SocketEntity socketEnt = new SocketEntity();
        socketEnt.angent = client;
        client.socket = socketEnt;
        client.network = this;
        Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        socketEnt.socket = socket;

        IPAddress addr = IPAddress.Parse(ip);
        IPEndPoint ipe = new IPEndPoint(addr, port);
        socketEnt.ipPoint = ipe;
        IAsyncResult result = socket.BeginConnect(ipe, this.OnConnect, client);
        return client;
    }

    public SocketListener Listen(short port)
    {
        return new SocketListener();
    }

    public void reConnect(SocketClient client)
    {
        SocketEntity socketEnt = new SocketEntity();
        Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        IAsyncResult result = socket.BeginConnect(socketEnt.ipPoint, this.OnConnect, client);
        client.socket = socketEnt;
        socketEnt.socket = socket;
    }

    void OnConnect(IAsyncResult ar)
    {
        SocketClient client = ar.AsyncState as SocketClient;
        SocketEntity socketEnt = client.socket;
        Socket socket = socketEnt.socket;
        try
        {
            socket.EndConnect(ar);
            socketEnt.offset = 0;
            socketEnt.packet = 0;
            socket.BeginReceive(socketEnt.buffer.buffer, socketEnt.buffer.writePosition, socketEnt.buffer.writeLength, SocketFlags.None, new AsyncCallback(this.OnReceive), client);
            socketEnt.postSend();

            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.CONNECT;
            dispatchEvent(socketEnt, e);
        }
        catch (SocketException ex)
        {
            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.EXCEPTION;
            e.data = System.Text.Encoding.Default.GetBytes(ex.Message);
            dispatchEvent(socketEnt, e);
        }
     
    }

    void OnDisconnect(IAsyncResult ar)
    {
        SocketClient client = ar.AsyncState as SocketClient;
        SocketEntity socketEnt = client.socket;
        Socket socket = socketEnt.socket;
        try
        {
            socket.EndDisconnect(ar);
            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.EXIT;
            dispatchEvent(socketEnt, e);
        }
        catch
        {

        }
    }

    public void dispatchEvent(SocketEntity angent, SocketEvent e)
    {
        handle.WaitOne();
        eventList.Enqueue(e);
        clientList.Enqueue(angent);
        handle.Set();
    }

    void OnReceive(IAsyncResult ar)
    {
        SocketAngent client = ar.AsyncState as SocketAngent;
        SocketEntity socketEnt = client.socket;
        Socket socket = socketEnt.socket;
        try
        {
            int BytesRead = socket.EndReceive(ar);
            //if (BytesRead == 0)
            //    return;

            int count = 0;
            if (BytesRead <= 0)
            {
                socket.BeginDisconnect(true, new AsyncCallback(this.OnDisconnect), client);
                return;
            }
            socketEnt.buffer.writePosition = socketEnt.buffer.writePosition + BytesRead;
            while (true)
            {
                if (socketEnt.buffer.Read(socketEnt.countBytes, 0, sizeof(int)))
                {
                    count = System.BitConverter.ToInt32(socketEnt.countBytes, 0);  
                    count = BinaryStream.ntohl(count);
                    if (socketEnt.buffer.validDataLength >= count)
                    {
                        if (socketEnt.buffer.Pop(socketEnt.recvPacket, 0, count))
                        {
                            MemoryStream read_stream = new MemoryStream(socketEnt.recvPacket, sizeof(int), count - sizeof(int));
                            this.OnReceive(read_stream.ToArray(), socketEnt);

                        }
                        else
                            break;
                    }
                    else
                        break;
                }
                else
                    break;
            }
            if (socketEnt.buffer.writeLength <= 0)
            {
                return;
            }
            socket.BeginReceive(socketEnt.buffer.buffer, socketEnt.buffer.writePosition, socketEnt.buffer.writeLength, 0, new AsyncCallback(this.OnReceive), client);
        }
        catch (SocketException)
        {
            this.OnDisconnect(ar);
        }
    }

    void OnReceive(byte[] data, SocketEntity socketEnt)
    {

        //MemoryStream mem = new MemoryStream(data);

        //byte[] outBytes = null;
        //int outCount = 0;
        //angent.unCompression(data, data.Length, ref outBytes, ref outCount);

        //MemoryStream read_mem = new MemoryStream(outBytes, 0, outCount);


        SocketEvent e = new SocketEvent();
        e.name = SocketEvent.RECV;
        e.data = data;
        dispatchEvent(socketEnt, e);
    }

	void Update () 
    {
	
	}
}

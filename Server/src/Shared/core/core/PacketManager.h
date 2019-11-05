#pragma once

class Packet;

class PacketHelper
{
public:
	PacketHelper() {}
	virtual ~PacketHelper() {}
	virtual void* Alloc() = 0;
	virtual void Free(void*& ptr) = 0;
	virtual const char* GetName() = 0;
};

template<class T>
class PacketRegister : public PacketHelper
{
public:
	PacketRegister(const std::string& name) { mName = name; };
	virtual ~PacketRegister() { }
	virtual void* Alloc() {  return new T; }
	virtual void Free(void*& ptr) { delete ((T*)ptr); ptr = NULL; }
	virtual const char* GetName() { return mName.c_str(); }
protected:
	std::string			mName;
};

class PacketManager
{
public:
	PacketManager(void);
	virtual ~PacketManager(void);
	bool RegisterPacket(uint32 msgId, PacketHelper* helper);
	bool UnregisterPacket(uint32 msgId);
	Packet* Alloc(uint32 msgId);
	void Free(Packet*& pack);
	std::string GetName(uint32 msgId);
	PacketHelper* Find(uint32 msgId);
private:
	std::set<Packet*>										mPacketSet;
	std::map<uint32, PacketHelper*>							mMapPacketHelper;
};

#define REGISTER_PACKET_HELPER(ID, classname)			\
struct stPacket##classname{\
stPacket##classname(){\
PacketHelper* pHelper = new PacketRegister<classname>(#classname);\
INSTANCE(PacketManager).RegisterPacket(ID, pHelper);}};\
static stPacket##classname g_stPacket##classname;


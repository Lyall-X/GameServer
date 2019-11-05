#include "Shared.h"
#include "Singleton.h"
#include "Object.hpp"
#include "Packet.h"
#include "PacketManager.h"

PacketManager::PacketManager( void )
{
	mMapPacketHelper.clear();
}

PacketManager::~PacketManager( void )
{
	std::set<Packet *> packetSet = mPacketSet;
	for (Packet* aPacket : packetSet)
		Free(aPacket);
	mPacketSet.clear();

	for (auto itr : mMapPacketHelper)
		delete itr.second;
	mMapPacketHelper.clear();
}

bool PacketManager::RegisterPacket(uint32 msgId, PacketHelper* helper )
{
	if (Find(msgId) != NULL)
	{
		//printf("id:[0x%p] name:[%s] packet repeat register\n", uiID, pHelper->name());
		delete helper;
		return false;
	}

	mMapPacketHelper[msgId] = helper;
	return true;
}

bool PacketManager::UnregisterPacket(uint32 msgId)
{
	auto itr = mMapPacketHelper.find(msgId);
	if (itr == mMapPacketHelper.end())
		return false;
	delete itr->second;
	mMapPacketHelper.erase(itr);
	return true;
}

Packet* PacketManager::Alloc(uint32 msgId)
{
	PacketHelper* packet_helper = Find(msgId);
	if (packet_helper == NULL)
		return NULL;
	Packet* aPacket = (Packet*)packet_helper->Alloc();
	if (aPacket)
		mPacketSet.insert(aPacket);
	return aPacket;
}

void PacketManager::Free(Packet*& pack)
{
	auto itr = mMapPacketHelper.find(pack->getMsgId());
	if (itr == mMapPacketHelper.end()) return;
	mPacketSet.erase(pack);
	PacketHelper* aPacketHelper = itr->second;
	if (aPacketHelper){
		aPacketHelper->Free((void*&)pack);
		pack = NULL;
	}
}

std::string PacketManager::GetName(uint32 uiID)
{
	auto itr = mMapPacketHelper.find(uiID);
	if (itr != mMapPacketHelper.end())
		return itr->second->GetName();
	return "";
}

PacketHelper* PacketManager::Find(uint32 uiID )
{
	auto itr = mMapPacketHelper.find(uiID);
	if (itr != mMapPacketHelper.end())
		return itr->second;
	return NULL;
}

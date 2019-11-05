#include "stdafx.h"

IMPLEMENT_CLASS(WorldModule, Module);
WorldModule::WorldModule()
{

}

WorldModule::~WorldModule()
{
	Destroy();
}

bool WorldModule::Initialize()
{
	sRedisProxy.addEventListener("OnRedisAuth", (EventCallback)&WorldModule::onRedisAuth, this);
	return true;
}

bool WorldModule::Update(float time, float delay)
{
	float64 s0 = DateTime::GetNowAppUS();

	std::queue<Entity* > delQueue;
	for (auto& itr : mMapEntity) {
		Entity* ent = itr.second;
		ent->Update(time, delay);
		if (ent->CanDestroy()) {
			delQueue.push(ent);
		}
	}

	while (delQueue.size() > 0)
	{
		Entity* ent = delQueue.front();
		delQueue.pop();
		ent->Destroy();
	}

	//float64 s1 = DateTime::GetNowAppUS() - s0;
	//if (s1 >= 0.02f)
	//{
	//	LOG_ERROR("Update delay: %0.3f", s1);
	//}
	return true;
}

bool WorldModule::Destroy()
{
	ClearEntity();
	ClearPlayerRecord();
	return true;
}

bool WorldModule::onEnterWorld(Player* player, Dictionary& dict)
{
	LOG_DEBUG(LogSystem::csl_color_blue, "player [%s] enter world", player->getName());
	PlayerRecord* aPlrRcd = FindPlrRecord(player->getUserId());
	if (aPlrRcd == NULL)
	{
		aPlrRcd = new PlayerRecord;
		aPlrRcd->mUserId = player->getUserId();
		aPlrRcd->mPlayer = player;
		AddPlrRecord(aPlrRcd);
	}
	else {
		aPlrRcd->mPlayer = player;
	}

	return true;
}

bool WorldModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	if (player->CanDestroy())
		LOG_DEBUG(LogSystem::csl_color_blue, "player [%s] destroy", player->getName());
	else
		LOG_DEBUG(LogSystem::csl_color_blue, "player [%s] leave world", player->getName());

	PlayerRecord* aPlrRcd = FindPlrRecord(player->getUserId());
	if (aPlrRcd) {
		if (player->CanDestroy()) {
			aPlrRcd->mPlayer = NULL;
		}
	}

	return true;
}

Entity* WorldModule::FindEntByName(cstring& name)
{
	auto itr = mMapNameEntity.find(name);
	if (itr != mMapNameEntity.end())
		return itr->second;

	return NULL;
}

Entity* WorldModule::FindEntByGuid(Guid guid)
{
	auto itr = mMapEntity.find(guid);
	if (itr != mMapEntity.end())
		return itr->second;

	return NULL;
}

Player* WorldModule::addPlayerByAccId(Player* plr)
{
	addEntity(plr);
	auto itr = mMapPlayer.find(plr->getAccId());
	if (itr != mMapPlayer.end())
		return NULL;

	mMapPlayer.insert(std::make_pair(plr->getAccId(), plr));
	return plr;
}

Player* WorldModule::addPlayerByName(Player* plr)
{
	return (Player*)addEntityByName(plr);
}

Entity* WorldModule::addEntity(Entity* ent)
{
	auto itr = mMapEntity.find(ent->getGuid());
	if (itr != mMapEntity.end())
		return NULL;

	mMapEntity.insert(std::make_pair(ent->getGuid(), ent));
	return ent;
}

Entity* WorldModule::addEntityByName(Entity* ent)
{
	auto itr = mMapNameEntity.find(ent->getName());
	if (itr != mMapNameEntity.end())
		return NULL;

	mMapNameEntity.insert(std::make_pair(ent->getName(), ent));
	return ent;
}

void WorldModule::removeEntity(Guid guid)
{
	auto itr = mMapEntity.find(guid);
	if (itr == mMapEntity.end())
		return;
	removeEntity(itr->second);
}

void WorldModule::removeEntity(Entity* ent)
{
	mMapEntity.erase(ent->getGuid());
	mMapNameEntity.erase(ent->getName());
	//delete ent;
	sObjectMgr.FreeObject(ent);
}

void WorldModule::removePlayer(uint32 accId)
{
	auto itr = mMapPlayer.find(accId);
	if (itr == mMapPlayer.end())
		return;

	Player* plr = itr->second;
	if (plr == NULL)
		return;
	removePlayer(plr);
}

void WorldModule::removePlayer(Player* plr)
{
	removePlayerToUserId(plr->getUserId());
	mMapPlayer.erase(plr->getAccId());
	removeEntity(plr->getGuid());
}

Player* WorldModule::addPlayerToUserId(Player* plr)
{
	auto itr = mMapUserIdPlayer.find(plr->getUserId());
	if (itr != mMapUserIdPlayer.end())
		return NULL;

	mMapUserIdPlayer.insert(std::make_pair(plr->getUserId(), plr));
	return plr;
}

void WorldModule::removePlayerToUserId(uint32 userId)
{
	auto itr = mMapUserIdPlayer.find(userId);
	if (itr != mMapUserIdPlayer.end())
		mMapUserIdPlayer.erase(itr);
}

Player* WorldModule::FindPlrByName(cstring& name)
{
	Entity* entity = FindEntByName(name);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Player)
		return (Player*)entity;
	return NULL;
}

Player* WorldModule::FindPlrByGuid(Guid guid)
{
	Entity* entity = FindEntByGuid(guid);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Player)
		return (Player*)entity;
	return NULL;
}

Player* WorldModule::FindPlrByAccId(uint32 accId)
{
	auto itr = mMapPlayer.find(accId);
	if (itr != mMapPlayer.end())
		return itr->second;

	return NULL;
}

Player* WorldModule::FindPlrByUserId(uint32 userId)
{
	auto itr = mMapUserIdPlayer.find(userId);
	if (itr != mMapUserIdPlayer.end())
		return itr->second;
	return NULL;
}

PlayerRecord* WorldModule::AddPlrRecord(PlayerRecord* aPlrRecord)
{
	if (FindPlrRecord(aPlrRecord->GetUserId())) return NULL;
	mMapPlrRecords[aPlrRecord->GetUserId()] = aPlrRecord;
	mMapPlayerNameRecord[aPlrRecord->GetName()] = aPlrRecord;
	return aPlrRecord;
}

PlayerRecord* WorldModule::FindPlrRecord(uint32 userId)
{
	auto itr = mMapPlrRecords.find(userId);
	if (itr == mMapPlrRecords.end()) return NULL;
	return itr->second;
}

PlayerRecord* WorldModule::FindPlrRecord(cstring& name)
{
	auto itr = mMapPlayerNameRecord.find(name);
	if (itr == mMapPlayerNameRecord.end()) return NULL;
	return itr->second;
}

void WorldModule::ClearPlayerRecord()
{
	for (auto& itr : mMapPlrRecords) {
		delete itr.second;
	}
	mMapPlrRecords.clear();
}

void WorldModule::ClearEntity()
{
	while (mMapEntity.size() > 0)
	{
		auto itr = mMapEntity.begin();
		removeEntity(itr->second->getGuid());
	}
}

void WorldModule::DelPlrRecord(uint32 userId)
{
	auto itr = mMapPlrRecords.find(userId);
	if (itr == mMapPlrRecords.end()) return;
	delete itr->second;
	mMapPlrRecords.erase(itr);
}

Npc* WorldModule::addNpc(Npc* npc)
{
	addEntity(npc);
	auto itr = mMapNpc.find(npc->getNpcId());
	if (itr != mMapNpc.end())
		return NULL;

	mMapNpc.insert(std::make_pair(npc->getNpcId(), npc));
	return npc;
}

Npc* WorldModule::FindNpcByCfgId(uint32 cfgId)
{
	auto itr = mMapNpc.find(cfgId);
	if (itr != mMapNpc.end())
		return itr->second;
	return NULL;
}

Npc* WorldModule::FindNpcByName(cstring& name)
{
	Entity* entity = FindEntByName(name);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Npc)
		return (Npc*)entity;
	return NULL;
}

Npc* WorldModule::FindNpcByGuid(Guid guid)
{
	Entity* entity = FindEntByGuid(guid);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Npc)
		return (Npc*)entity;
	return NULL;
}

void WorldModule::DestroyEnt(Guid guid)
{
	Entity* ent = FindEntByGuid(guid);
	if (ent == NULL) return;
	DestroyEnt(ent);
}

void WorldModule::DestroyEnt(Entity* ent)
{

}

void WorldModule::removeNpc(uint32 npcId)
{
	auto itr = mMapNpc.find(npcId);
	if (itr == mMapNpc.end())
		return;

	Npc* npc = itr->second;
	if (npc == NULL)
		return;
	removeNpc(npc);
}

void WorldModule::removeNpc(Npc* npc)
{
	mMapNpc.erase(npc->getNpcId());
	removeEntity(npc->getGuid());
}

bool WorldModule::ChangeName(Entity* ent, cstring& sname)
{
	if (ent->GetNameStr() == sname) return false;
	if (FindPlrRecord(sname))
		return false;
	{
		auto itr = mMapNameEntity.find(ent->getName());
		if (itr == mMapNameEntity.end())
			return false;
		mMapNameEntity.erase(itr);
	}
	
	std::string oldName = ent->getName();
	ent->setName(sname);
	mMapNameEntity[ent->getName()] = ent;

	{
		auto itr = mMapPlayerNameRecord.find(oldName);
		if (itr != mMapPlayerNameRecord.end())
		{
			mMapPlayerNameRecord[sname] = itr->second;
			mMapPlayerNameRecord.erase(itr);
		}
	}

	if (ent->getEntityType() == ET_Player) {
		Player* aPlr = (Player*)ent;
		aPlr->AddOldName(oldName);
		Dictionary dict;
		sApp.doPlayerSave(aPlr, dict);
	}

	Event e;
	e.event = "OnChangeName";
	e.params.push_back((void*)oldName.c_str());
	e.params.push_back((void*)ent->getName());
	sWorld.Dispatch(e);
	return true;
}

void WorldModule::sendPacketToAll(Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (auto itr : mMapPlayer)
		itr.second->sendBuffer(in.datas(), in.wpos());
}

void WorldModule::sendPacketToTarget(EnumChannel ec, Packet& packet, Player* self, uint32 tarUserId /* = 0 */)
{
	switch (ec)
	{
	case EC_WORLD:
		self->sendPacketToWorld(packet);
		break;
	case EC_SYSTEM:
		self->sendPacket(packet);
		break;
	case EC_MAP:
		self->sendPacketToMap(packet);
		break;
	case EC_VIEW:
		self->sendPacketToView(packet);
		break;
	case EC_TEAM:
		self->sendPacketToTeam(packet);
		break;
	case EC_ROOM:
		self->sendPacketToRoom(packet);
		break;
	case EC_TARGET:
		if (tarUserId == 0)
			self->sendPacket(packet);
		else
		{
			Player* tarPlr = sWorld.FindPlrByUserId(tarUserId);
			if (tarPlr)
				tarPlr->sendPacket(packet);
		}
		break;
	}
}

void WorldModule::sendPacketToMsg(EnumChannel ec, const std::string& msg, Player* self)
{
	NetChatMsgNotify nfy;
	nfy.chatMsg = msg;
	nfy.channelType = ec;
	nfy.from = self->getName();
	sendPacketToTarget((EnumChannel)ec, nfy, self);
}

int32 WorldModule::onRedisAuth(Event& e)
{
	char szBuffer[64] = { 0 };
	sprintf_s(szBuffer, 64, "hgetall %s", sUser);
	float64 s0 = DateTime::GetNowAppUS();
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&WorldModule::onRedisAllPlr, this);

	return 0;
}

int32 WorldModule::onRedisAllPlr(RedisEvent& e)
{
	float64 s0 = DateTime::GetNowAppUS();
	for (uint32 i = 0; i < e.backstr.size(); i += 2)
	{
		std::string keystr = e.backstr[i];
		std::string valuestr = e.backstr[i + 1];
		uint32 plrUserId = Shared::strtoint32(keystr);
		if (FindPlrRecord(plrUserId)) {
			continue;
		}

		Json::Reader jsonReader;
		Json::Value root;
		jsonReader.parse(valuestr.c_str(), root);
		Json::Value userJson = root["user"];
		PlayerRecord* aPlrRecd = new PlayerRecord;
		aPlrRecd->mUserId = userJson["userId"].asUInt();
		aPlrRecd->mName = userJson["name"].asString();
		AddPlrRecord(aPlrRecd);
	}
	MapPlayerRecord& mapPlayer = GetMapPlayer();
	LOG_DEBUG(LogSystem::csl_color_green, "加载%u个玩家数据完成!", mapPlayer.size());
	return 0;
}

uint32 PlayerRecord::GetUserId()
{
	if (mPlayer) {
		return mPlayer->getUserId();
	}
	return mUserId;
}

const std::string& PlayerRecord::GetName()
{
	if (mPlayer == NULL) return mName;
	return mPlayer->GetNameStr();
}

bool PlayerRecord::GetOnline()
{
	if (mPlayer == NULL) return false;
	return mPlayer->GetOnline();
}

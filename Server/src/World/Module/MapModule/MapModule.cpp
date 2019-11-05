#include "stdafx.h"

IMPLEMENT_CLASS(MapModule, Module);
MapModule::MapModule()
{

}

MapModule::~MapModule()
{
	while (mMapMap.size() > 0)
	{
		auto itr = mMapMap.begin();
		delete itr->second;
		mMapMap.erase(itr);
	}
}

bool MapModule::Initialize()
{
	std::map<int32, MapJson>& mapMapJson = sCfgMgr.getMapMapJson();

	for (auto itr : mapMapJson)
	{
		MapJson& mapJson = itr.second;
		if (mapJson.Type == MT_BigMap)
		{
			Map* aMap = new Map;
			aMap->setMapCfgId(mapJson.ID);
			aMap->Initalize();
			if (addMap(aMap)) {
				LOG_DEBUG(LogSystem::csl_color_green, "create map success id:%d instanceId:%d ", aMap->getMapId(), aMap->getGuid());
			}
			else {
				delete aMap;
			}
		}
	}

	for (auto itr : mMapBigMap)
	{
		Map* map = itr.second;
		MapJson* mapJson = sCfgMgr.getMapJson(map->getMapId());
		if (mapJson == NULL) continue;

		std::vector<NpcJson*>* npcJsons = INSTANCE(ConfigManager).getNpcJsons(mapJson->ID);
		if (npcJsons)
		{
			for (uint32 i = 0; i < npcJsons->size(); ++i)
			{
				NpcJson* npcJson = (*npcJsons)[i];
				CharJson* charJosn = INSTANCE(ConfigManager).getCharJson(npcJson->CharId);
				if (!charJosn)
				{
					LOG_ERROR("not exist %d", npcJson->CharId);
					continue;
				}
				Npc* npc = new Npc;
				npc->setSex(charJosn->Sex);
				npc->setNpcId(npcJson->ID);
				npc->setNpcType(npcJson->Type);
				npc->setName(npcJson->Name);
				npc->setCharId(npcJson->CharId);
				npc->setCellX(npcJson->PosX);
				npc->setCellY(npcJson->PosY);
				npc->setCellTarX(npc->getCellX());
				npc->setCellTarY(npc->getCellY());
				npc->setDirPosition(D_DOWNRIGHT);

				sWorld.addNpc(npc);
				map->onEntityEnter(npc);
			}
		}
	}

	return true;
}

bool MapModule::Update(float time, float delay)
{
	return true;
}

bool MapModule::Destroy()
{
	return true;
}

bool MapModule::onEnterWorld(Player* player, Dictionary& dict)
{
	if (dict.ContainsKey(ep_mapId))
	{
		int32 mapId = dict[ep_mapId].valueUint32();
		player->changeMapByMapId(mapId);
	}

	return true;
}

bool MapModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	Map* map = player->getMap();
	if (map)
		player->changeMapByMap(NULL);
	return true;
}

Map* MapModule::getMap(int32 instanceId)
{
	auto itr = mMapMap.find(instanceId);
	if (itr != mMapMap.end())
		return itr->second;

	return NULL;
}

Map* MapModule::getMapByMapId(int32 mapId)
{
	auto itr = mMapBigMap.find(mapId);
	if (itr != mMapBigMap.end())
		return itr->second;

	return NULL;
}

Map* MapModule::addMap(Map* map)
{
	auto itr = mMapMap.find(map->getMapId());
	if (itr != mMapMap.end())
		return NULL;

	if (map->getMapType() == MT_BigMap)
		mMapBigMap.insert(std::make_pair(map->getMapId(), map));

	mMapMap.insert(std::make_pair(map->getMapId(), map));
	return map;
}

void MapModule::removeMap(uint32 id)
{
	auto itr = mMapMap.find(id);
	if (itr != mMapMap.end())
	{
		mMapBigMap.erase(itr->second->getMapId());
		delete itr->second;
		mMapMap.erase(itr);
	}
}


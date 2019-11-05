#pragma once

class MapModule : public Module
{
	DECLARE_CLASS(MapModule)
public:
	MapModule();
	virtual ~MapModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	Map* getMap(int32 instanceId);
	Map* getMapByMapId(int32 mapId);
protected:
	Map* addMap(Map* map);
	void removeMap(uint32 id);
protected:
	std::map<uint32, Map*>			mMapMap;
	std::map<uint32, Map*>			mMapBigMap;

};
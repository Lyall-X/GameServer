#pragma once

enum MapType
{
	MT_None = 0,
	MT_BigMap = 1,
	MT_CopyMap = 2,

};
class Entity;
class Packet;

typedef std::map<uint8, EntitySet> LineEntitySet;
class MapCell
{
public:
	LineEntitySet	lineObjects;
	EntitySet		objects;
};

class Map
{
public:
	Map();
	~Map();
public:
	uint32 getGuid() { return mInstanceId; }
	uint32 getMapId() { return mMapCfgId; }
	void setMapCfgId(uint32 cfgId) { mMapCfgId = cfgId; }
	uint8 getMapType();

	bool Initalize();

	virtual bool onEntityEnter(Entity* entity);
	virtual bool onEntityLeave(Entity* entity);

	// tar enter ent;
	virtual bool onEntityEnterView(Entity* entity, Entity* target);
	virtual bool onEntityLeaveView(Entity* entity, Entity* target);

	Entity* getEntity(Guid guid);
	Entity* addEntity(Entity* entity);
	void removeEntity(Guid guid);

	bool checkUnitView(Entity* entity, Entity* target);
	void checkMapUnitView(Entity* entity);

	void checkMapEntityView(Entity* entity);
	bool getEntityMapCells(Entity* ent, std::vector<MapCell*>& mapCells);
	void sendPacketToAll(Packet& packet);

	bool MapCellToMapLogicCell(int32 x, int32 y, int32& outX, int32& outY);
	MapCell* getMapLogicCellByPos(int32 x, int32 y);
	MapCell* getMapLogicCell(int32 x, int32 y);
	void addEntityToMapCell(Entity* entity, int32 x, int32 y);
	void addEntityToMapCell(Entity* entity, MapCell* cell);
	void delEntityToMapCell(Entity* entity, int32 x, int32 y);
	void delEntityToMapCell(Entity* entity, MapCell* cell);
	uint8 getEnterLine();
public:
	float32 getMapCell() { return mCellSize; }
	uint32 getRow() { return mRow; }
	uint32 getCol() { return mCol; }
	int8* getMapBytes() { return mMapBytes; }
	bool changeLine(Entity* ent, uint8 line);
protected:
	void addEntityToLine(uint8 line, Entity* ent);
	void delEntityToLine(uint8 line, Entity* ent);
	EntitySet* getEntityToLine(uint8 line);
	uint8 getEntityLine(Guid guid);
	bool checkEntityLine(Entity* ent, Entity* tar);
protected:
	static	uint32	sId;
protected:
	std::map<Guid, Entity*>					mMapEntity;
	std::map<Guid, uint8>					mMapEntityLine;
	std::map<uint8, EntitySet>				mMapEntitySetLine;
public:
	uint32			mInstanceId;
	uint32			mMapCfgId;
	uint32			mRow;
	uint32			mCol;
	uint32			mCellRow;
	uint32			mCellCol;
	float32			mCellSize;
	int8*			mMapBytes;
	MapCell*		mMapCells;
	int32			mViewCell;
	Astar			mAstar;
public:
};
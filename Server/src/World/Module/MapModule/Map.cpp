#include "stdafx.h"

void Map::addEntityToLine(uint8 line, Entity* ent)
{
	mMapEntityLine.insert(std::make_pair(ent->getGuid(), line));
	ent->setLine(line);
	auto itr = mMapEntitySetLine.find(line);
	if (itr != mMapEntitySetLine.end())
	{
		EntitySet& entSet = itr->second;
		entSet.insert(ent);
		return;
	}

	EntitySet entSet;
	entSet.insert(ent);
	mMapEntitySetLine.insert(std::make_pair(line, entSet));
}

void Map::delEntityToLine(uint8 line, Entity* ent)
{
	ent->setLine(0);
	mMapEntityLine.erase(ent->getGuid());
	auto itr = mMapEntitySetLine.find(line);
	if (itr == mMapEntitySetLine.end()) return;
	
	EntitySet& entSet = itr->second;
	entSet.erase(ent);
	if (entSet.size() <= 0)
		mMapEntitySetLine.erase(itr);
}

EntitySet* Map::getEntityToLine(uint8 line)
{
	auto itr = mMapEntitySetLine.find(line);
	if (itr != mMapEntitySetLine.end())
		return &itr->second;
	return NULL;
}

uint8 Map::getEntityLine(Guid guid)
{
	auto itr = mMapEntityLine.find(guid);
	if (itr != mMapEntityLine.end())
		return itr->second;
	return 0;
}

bool Map::checkEntityLine(Entity* ent, Entity* tar)
{
	if (ent->getLine() == 0 || tar->getLine() == 0) return true;
	return ent->getLine() == tar->getLine();
}

uint32 Map::sId = 0;
Map::Map():
mInstanceId(++sId),
mMapCfgId(0),
mMapBytes(NULL),
mMapCells(NULL)
{

}

Map::~Map()
{
	SAFE_DELETE_ARRAY(mMapBytes);
	SAFE_DELETE_ARRAY(mMapCells);
}

uint8 Map::getMapType()
{
	MapJson* cfg = sCfgMgr.getMapJson(getMapId());
	if (cfg == NULL) return 0;
	return cfg->Type;
}

bool Map::Initalize()
{
	SAFE_DELETE_ARRAY(mMapBytes);
	SAFE_DELETE_ARRAY(mMapCells);
	MapJson* cfg = sCfgMgr.getMapJson(getMapId());
	if (cfg == NULL) return false;

	mRow = cfg->Row;
	mCol = cfg->Col;
	mCellSize = cfg->Cell;
	mCellRow = cfg->LogicRow;
	mCellCol = cfg->LogicCol;
	mViewCell = cfg->ViewCell;

	mMapBytes = new int8[mRow * mCol];
	memset(mMapBytes, 0, mRow * mCol);
	mMapCells = new MapCell[mCellRow * mCellCol];
	mAstar.create(mRow, mCol, mMapBytes);

	mMapEntitySetLine.insert(std::make_pair(0, EntitySet()));
	return true;
}

bool Map::onEntityEnter(Entity* entity)
{
	entity->setMap(this);
	if (!addEntity(entity))
		return false;
	if (entity->getEntityType() == ET_Player) {
		uint8 line = getEnterLine();
		addEntityToLine(line, entity);
	}
	else {
		addEntityToLine(0, entity);
	}

	addEntityToMapCell(entity, entity->getCellX(), entity->getCellY());
	entity->onEnterMap();
	checkMapEntityView(entity);
	//checkMapUnitView(entity);
	return true;
}

bool Map::onEntityLeave(Entity* entity)
{
	EntitySet& lstView = entity->GetVisual().GetEntitySet();
	EntitySet copyView = lstView;
	for (auto itr : copyView)
	{
		onEntityLeaveView(itr, entity);
		onEntityLeaveView(entity, itr);
	}

	entity->onLeaveMap();
	delEntityToMapCell(entity, entity->getCellX(), entity->getCellY());
	removeEntity(entity->getGuid());

	uint8 line = getEntityLine(entity->getGuid());
	delEntityToLine(line, entity);

	entity->setMap(NULL);
	return true;
}

bool Map::onEntityEnterView(Entity* entity, Entity* target)
{
	if (entity->GetVisual().CheckView(target))
		return true;
	entity->GetVisual().EnterView(target);
	entity->onEnterView(target);

	return true;
}

bool Map::onEntityLeaveView(Entity* entity, Entity* target)
{
	if (!entity->GetVisual().CheckView(target))
		return true;

	target->onLeaveView(entity);
	entity->GetVisual().LeaveView(target);
	return true;
}

Entity* Map::getEntity(Guid guid)
{
	auto itr = mMapEntity.find(guid);
	if (itr != mMapEntity.end())
		return itr->second;
	return NULL;
}

Entity* Map::addEntity(Entity* entity)
{
	auto itr = mMapEntity.find(entity->getGuid());
	if (itr != mMapEntity.end())
		return NULL;

	mMapEntity.insert(std::make_pair(entity->getGuid(), entity));
	return entity;
}

void Map::removeEntity(Guid guid)
{
	auto itr = mMapEntity.find(guid);
	if (itr != mMapEntity.end())
		mMapEntity.erase(itr);
}

bool Map::checkUnitView(Entity* entity, Entity* target)
{
	Vector2 v0;
	v0.x = (float32)entity->getCellX();
	v0.y = (float32)entity->getCellY();
	Vector2 v1;
	v1.x = (float32)target->getCellX();
	v1.y = (float32)target->getCellY();
	if (Vector2::Distance(v0, v1) <= 28.f)
		return true;
	return false;
	//return true;
	//Rectangle uView(upro->pos.x - 500, upro->pos.y - 500, 1000, 1000);
	//return uView.Intersect(tpro->pos.x, tpro->pos.y);
}

void Map::checkMapUnitView(Entity* entity)
{
	for (auto itr : mMapEntity)
	{
		Entity* target = itr.second;
		if (checkUnitView(entity, target))
		{
			if (!entity->GetVisual().CheckView(target))
				onEntityEnterView(entity, target);

			if (!target->GetVisual().CheckView(entity))
				onEntityEnterView(target, entity);
		}
		else
		{
			if (entity->GetVisual().CheckView(target))
				onEntityLeaveView(entity, target);
			if (target->GetVisual().CheckView(entity))
				onEntityLeaveView(target, entity);
		}
	}
}

void Map::checkMapEntityView(Entity* entity)
{
	EntitySet& lstView = entity->GetVisual().GetEntitySet();
	EntitySet copyView = lstView;
	std::vector<MapCell*> mapCells;
	getEntityMapCells(entity, mapCells);

	for (auto mapCell : mapCells)
	{
		for (auto ent : mapCell->objects)
		{
			if (ent->getEntityType() != ET_Player &&
				entity->getEntityType() != ET_Player) {
				continue;
			}

			if (checkEntityLine(entity, ent))
			{
				auto entItr = copyView.find(ent);
				if (entItr != copyView.end() && entity->getLine() == ent->getLine())
					copyView.erase(entItr);
				else
				{
					onEntityEnterView(entity, ent);
					onEntityEnterView(ent, entity);
				}
			}
		}
	}

	for (auto ent : copyView)
	{
		if (ent->getEntityType() != ET_Player &&
			entity->getEntityType() != ET_Player) {
			continue;
		}
		onEntityLeaveView(entity, ent);
		onEntityLeaveView(ent, entity);
	}
}

bool Map::getEntityMapCells(Entity* ent, std::vector<MapCell*>& mapCells)
{
	int32 x = 0;
	int32 y = 0;
	MapCellToMapLogicCell(ent->getCellX(), ent->getCellY(), x, y);

	for (int32 i = x - mViewCell; i <= x + mViewCell; ++i)
	{
		for (int32 j = y - mViewCell; j <= y + mViewCell; ++j)
		{
			MapCell* cell = getMapLogicCell(i, j);
			if (!cell)
				continue;
			mapCells.push_back(cell);
		}
	}
	return true;
}

void Map::sendPacketToAll(Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (auto itr : mMapEntity)
		itr.second->sendBuffer(in.datas(), in.wpos());
}

bool Map::MapCellToMapLogicCell(int32 x, int32 y, int32& outX, int32& outY)
{
	outX = (int32)((float32)x / (mRow - 1) * (mCellRow - 1));
	outY = (int32)((float32)y / (mCol - 1) * (mCellCol - 1));
	return true;
}

MapCell* Map::getMapLogicCellByPos(int32 x, int32 y)
{
	int32 r = 0;
	int32 c = 0;
	MapCellToMapLogicCell(x, y, r, c);

	if (r < 0 || r >= mCellRow || c < 0 || c >= mCellCol)
		return NULL;

	return &mMapCells[r * mCellCol + c];
}

MapCell* Map::getMapLogicCell(int32 x, int32 y)
{
	if (x < 0 || x >= mCellRow || y < 0 || y >= mCellCol)
		return NULL;

	return &mMapCells[x * mCellCol + y];
}


void Map::addEntityToMapCell(Entity* entity, int32 x, int32 y)
{
	MapCell* cell = getMapLogicCellByPos(x, y);
	if (cell == NULL)
		return;
	addEntityToMapCell(entity, cell);
}

void Map::addEntityToMapCell(Entity* entity, MapCell* cell)
{
	if (cell == NULL)
		return;
	cell->objects.insert(entity);
}

void Map::delEntityToMapCell(Entity* entity, int32 x, int32 y)
{
	MapCell* cell = getMapLogicCellByPos(x, y);
	if (cell == NULL)
		return;
	delEntityToMapCell(entity, cell);
}

void Map::delEntityToMapCell(Entity* entity, MapCell* cell)
{
	if (cell == NULL)
		return;
	auto itr = std::find(cell->objects.begin(), cell->objects.end(), entity);
	if (itr == cell->objects.end())
		return;
	cell->objects.erase(itr);
}

uint8 Map::getEnterLine()
{
	MapJson* cfg = sCfgMgr.getMapJson(getMapId());
	if (cfg == NULL) return 0;
	uint8 maxLine = 0;
	for (auto itr : mMapEntitySetLine)
	{
		maxLine++;
		if (itr.first == 0) continue;
		EntitySet& entSet = itr.second;
		if (entSet.size() < cfg->LineMax)
			return itr.first;
	}
	return maxLine;
}

bool Map::changeLine(Entity* ent, uint8 line)
{
	delEntityToLine(ent->getLine(), ent);
	addEntityToLine(line, ent);
	checkMapEntityView(ent);
	return true;
}


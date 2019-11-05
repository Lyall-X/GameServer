#include "stdafx.h"

static Guid sId = 0;
Entity::Entity(int8 type /* = 0 */):
	mGuid(++sId),
entityType(type),
mSpeed(0.f),
mDirPosition(D_UP),
mCheckView(false),
mLine(0)
{
	mStatus = new FSM(this);
	mCheckTime = 0.f;

	Initialize();
}

Entity::~Entity()
{

}

bool Entity::Initialize()
{
	GetModule(WarModule)->CreateWar(this);
	mStatus->Register(ES_IDLE, new EntityIdleStatus);
	mStatus->Register(ES_MOVE, new EntityMoveStatus);
	mStatus->ChangeStatus(ES_IDLE);

	mTimer.addEventListener((EventCallback)&Entity::onTimerChangeProperty, this, 1.f);
	mTimer.addEventListener((EventCallback)&Entity::onTimerCheckView, this, 0.5f);
	return true;
}

bool Entity::Update(float time, float delay)
{
	mTimer.update(time, delay);
	mStatus->update(time, delay);
	return true;
}

bool Entity::Destroy()
{
	delete mStatus;
	mStatus = NULL;

	while (mMapProperty.size())
	{
		auto itr = mMapProperty.begin();
		delete itr->second;
		mMapProperty.erase(itr);
	}
	GetModule(WarModule)->DestroyWar(this);

	sWorld.removeEntity(getGuid());
	return true;
}

bool Entity::CanDestroy()
{
	return mCanDestroy;
}

bool Entity::changeMapByMapInsId(int32 mapInsId)
{
	Map* aMap = sMap.getMap(mapInsId);
	if (aMap == NULL) return false;
	return changeMapByMap(aMap);
}

bool Entity::changeMapByMapId(int32 mapId)
{
	Map* aMap = sMap.getMapByMapId(mapId);
	if (aMap == NULL) return false;
	return changeMapByMap(aMap);
}

bool Entity::changeMapByMap(Map* map)
{
	if (map && map == getMap())
		return false;
	
	if (getMap())
		getMap()->onEntityLeave(this);
	
	if (map)
		map->onEntityEnter(this);
	return true;
}

bool Entity::onEnterView(Entity* tar)
{
	return true;
}

bool Entity::onLeaveView(Entity* tar)
{
	// 离开视野会取消踉随;
	War* war = GetModule(WarModule)->getWar(getGuid());
	if (war == NULL)
		return true;

	if (war->getTarget() && war->getTarget() == tar)
	{
		war->setTarget(NULL);
		NetEntityCancelFollowNotify nfy;
		sendPacket(nfy);
	}

	return true;
}

void Entity::setCellPosition(Point2& pos)
{
	if (pos.x == mPos.x && pos.y == mPos.y)
		return;
	ChangePos(mPos.x, mPos.y, pos.x, pos.y);
	mPos = pos;

	//addChangeValue(ep_posX, mPos.x);
	//addChangeValue(ep_posY, mPos.y);
}

void Entity::setCellX(int32 x)
{
	if (mPos.x == x)
		return;
	ChangePos(mPos.x, mPos.y, x, mPos.y);
	mPos.x = x;
}

void Entity::setCellY(int32 y)
{
	if (mPos.y == y)
		return;

	ChangePos(mPos.x, mPos.y, mPos.x, y);
	mPos.y = y;
}

void Entity::setSpeed(float32 speed)
{
	if (mSpeed == speed)
		return;

	mSpeed = speed;
	addChangeValue(ep_speed, speed);
}

void Entity::setDirPosition(int8 dir)
{
	mDirPosition = dir;
	//addChangeValue(ep_dirPos, mDirPosition);
}

void Entity::ChangeStatus(int32 status)
{
	mStatus->ChangeStatus(status); 
}

void Entity::addChangeValue(const Variant& key, const Variant& value)
{
	mDictChangePro[key] = value;
}

uint32 Entity::getMapId()
{
	return mMap ? mMap->getMapId() : 0;
}

void Entity::setMapId(uint32 mapId)
{
	Map* m = GetModule(MapModule)->getMapByMapId(mapId);
	if (m)
		setMap(m);
}

uint32 Entity::getMapInsId()
{
	return mMap ? mMap->getGuid() : 0;
}

void Entity::setMapInsId(uint32 mapInsId)
{
	Map* m = GetModule(MapModule)->getMap(mapInsId);
	if (m)
		setMap(m);
}

cstring Entity::getGuidStr()
{
	return Shared::uint64tostr(getGuid());
}

Property* Entity::getProperty(const std::string& name)
{
	auto itr = mMapProperty.find(name);
	if (itr != mMapProperty.end())
		return itr->second;

	return NULL;
}

Property* Entity::addProperty(Property* property)
{
	auto itr = mMapProperty.find(property->getClassName());
	if (itr != mMapProperty.end())
		return NULL;

	mMapProperty.insert(std::make_pair(property->getClassName(), property));
	return property;
}

void Entity::removeProperty(const std::string& name)
{
	auto itr = mMapProperty.find(name);
	if (itr != mMapProperty.end())
	{
		delete itr->second;
		mMapProperty.erase(itr);
	}
}

Variant& Entity::getVariant(const Variant& value)
{
	return mDictProperty[value];
}

bool Entity::ContainsVariantKey(const Variant& key)
{
	return mDictProperty.ContainsKey(key);
}

void Entity::addVariant(const Variant& key, const Variant& value)
{
	mDictProperty.Add(key, value);
}

void Entity::removeVariant(const Variant& key)
{
	mDictProperty.Remove(key);
}

bool Entity::serializeProperty(BinaryStream& stream)
{
	CHECK_RETURN(stream << (int32)mMapProperty.size(), false);
	for (auto itr : mMapProperty)
	{
		Property* property = itr.second;
		CHECK_RETURN(stream << std::string(property->getClassName()), false);
		if (!(stream << property))
			return false;
	}

	return true;
}

bool Entity::deSerializeProperty(BinaryStream& stream)
{
	int32 propertyCount = 0;
	CHECK_RETURN(stream >> propertyCount, true);
	for (int32 i = 0; i < propertyCount; ++i)
	{
		std::string property = "";
		CHECK_RETURN(stream >> property, false);
		Property* pro = getProperty(property);
		if (!(stream >> (*pro)))
			return false;
	}
	return true;
}

bool Entity::MoveTo()
{
	Map* map = getMap();
	if (!map)
		return false;

	if (map->mAstar.find(getCellX(), getCellY(), getCellTarX(), getCellTarY()))
	{
		mPoints.clear();
		if (!Astar::points(mPoints, map->mAstar.end_node()))
			return false;
		mPoints.pop_front();
	}

	if (mPoints.size() > 0)
	{
		mStatus->ChangeStatus(ES_MOVE);
		return true;
	}
	return false;
}

int32 Entity::onTimerChangeProperty(TimerEvent& e)
{
	if (mDictChangePro.GetCount() <= 0)
		return 0;

	NetEntityPropertyNotify nfy;
	nfy.property << mDictChangePro;
	nfy.guid = getGuid();
	sendPacketToView(nfy);
	mDictChangePro.Clear();
	return 0;
}

int32 Entity::onTimerCheckView(TimerEvent& e)
{
	if (mCheckView)
	{
		float64 s0 = DateTime::GetNowAppUS();
		if (getMap())
			getMap()->checkMapEntityView(this);

		float64 s1 = DateTime::GetNowAppUS() - s0;

		if (s1 >= 0.01)
		{
			int32 c = GetVisual().GetViewCount();
			LOG_ERROR("[%s] %0.3f viewCount:%d", this->getName(), s1, c);
		}
		mCheckView = false;
	}

	return 0;
}

void Entity::ChangePos(int32 lastX, int32 lastY, int32 x, int32 y)
{
	Map* map = getMap();
	if (map == NULL)
		return;
	MapCell* sCell = map->getMapLogicCellByPos(lastX, lastY);
	MapCell* eCell = map->getMapLogicCellByPos(x, y);

	if (sCell != eCell)
	{
		map->delEntityToMapCell(this, sCell);
		map->addEntityToMapCell(this, eCell);
		return;
	}
	if (sCell == NULL)
		return;

	auto itr = std::find(sCell->objects.begin(), sCell->objects.end(), this);
	if (itr != sCell->objects.end())
		return;
	map->addEntityToMapCell(this, sCell);
}

bool Entity::isMoveTo()
{
	if (this->mTarPos.x == mPos.x && this->mTarPos.y == mPos.y)
		return false;

	return true;
}

//bool Entity::MoveTo(Vector2& tar)
//{
//	//mTargetPos = tar;
//
//	return true;
//}

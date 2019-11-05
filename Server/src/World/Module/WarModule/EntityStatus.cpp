#include "stdafx.h"
int32 EntityIdleStatus::onEnter()
{
	return mStatus;
}

int32 EntityIdleStatus::onLeave()
{
	return mStatus;
}

int32 EntityIdleStatus::onUpdate(float time, float delay)
{
	return mStatus;
}

int32 EntityMoveStatus::onEnter()
{
	Entity* ent = (Entity*)mTarget;
	getPoint(ent);
	return mStatus;
}

int32 EntityMoveStatus::onLeave()
{
	return mStatus;
}

int32 EntityMoveStatus::onUpdate(float time, float delay)
{
	Entity* ent = (Entity*)mTarget;
	//War* war = GetModule(WarModule)->getWar(ent);
	//if (war)
	//{
	//	Entity* tar = war->getTarget();
	//	if (tar == NULL)
	//		return mStatus;

	//	Map* map = ent->getMap();
	//	if (map == NULL)
	//		return mStatus;

	//	if (ent->getCellTarPos().x != tar->getCellPos().x || ent->getCellTarPos().y != tar->getCellPos().y &&
	//		ent->getCellPos().distance(tar->getCellPos()) > 1)
	//	{
	//		ent->setCellTarPos(tar->getCellPos());
	//		ent->MoveTo();
	//		return mStatus;
	//	}
	//}
	float32 lastDis = ent->mPosition.distance(ent->mTarPosition);

	Vector2 pos = ent->mPosition + ent->mDir * ent->mSpeed * delay;
	float32 currentDis = pos.distance(ent->mTarPosition);
	if (currentDis < 0.5f || lastDis < currentDis)
	{
		if (ent->getPoints().size() <= 0)
			ent->setPos(ent->getTarPos());
		getPoint(ent);
	}
	else
	{
		ent->setPos(pos);
	}

	return mStatus;
}

void EntityMoveStatus::getPoint(Entity* ent)
{
	std::list<Point2>& points = ent->getPoints();
	if (points.size() <= 0)
	{
		ent->ChangeStatus(ES_IDLE);
		return;
	}
	Map* map = ent->getMap();
	if (!map)
		return;

	Point2 pt = points.front();
	points.pop_front();

	float32 cell = map->getMapCell();
	ent->setTarPos(Utils::Point2ToVector2(pt, cell, cell));

	ent->setDir(ent->getTarPos() - ent->getPos());
	ent->mDir.normalise();

	ent->setDirPosition(Utils::CalculateDir(ent->mDir));
	ent->setCellPosition(pt);
	ent->setCheckView(true);
}

int32 EntityAttackStatus::onEnter()
{
	return mStatus;
}

int32 EntityAttackStatus::onLeave()
{
	Entity* ent = (Entity*)mTarget;
	War* war = GetModule(WarModule)->getWar(ent);
	if (war == NULL)
		return mStatus;
	war->cancelSkill();

	return mStatus;
}

int32 EntityAttackStatus::onUpdate(float32 time, float32 delay)
{
	return mStatus;
}

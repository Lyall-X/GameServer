#include "stdafx.h"
Npc::Npc():
Entity(ET_Npc),
mNpcId(0)
{
	addProperty(new EntityProperty);
}

Npc::~Npc()
{

}

bool Npc::Destroy()
{
	sWorld.removeNpc(getNpcId());
	return true;
}

bool Npc::onEnterMap()
{
	LOG_DEBUG(LogSystem::csl_color_red_blue, "npc:%s enter mapId:%d x:%d y:%d", getName(), getMapId(), getCellX(), getCellY());
	return true;
}

bool Npc::onLeaveMap()
{
	return true;
}

bool Npc::onEnterPlayerView(Player* plr)
{
	NetNpcEnterViewNotify nfy;
	nfy.guid = this->getGuid();
	nfy.name = this->getName();
	nfy.npcId = this->getNpcId();
	nfy.entityType = this->getEntityType();
	nfy.mapId = this->getMapId();
	nfy.pos.x = this->getCellX();
	nfy.pos.y = this->getCellY();
	nfy.dir = this->getDirPosition();
	nfy.speed = this->getSpeed();
	nfy.status = this->mStatus->getCurrent();
	plr->sendPacket(nfy);

	if (!isMoveTo())
		return true;
	NetEntityMoveToNotify moveNfy;
	moveNfy.guid = this->getGuid();
	moveNfy.x = mTarPos.x;
	moveNfy.y = mTarPos.y;
	plr->sendPacket(moveNfy);
	return true;
}

bool Npc::onLeavePlayerView(Player* plr)
{
	NetNpcLeaveViewNotify nfy;
	nfy.guid = this->getGuid();
	nfy.mapId = getMapId();
	plr->sendPacket(nfy);
	return true;
}


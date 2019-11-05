#include "stdafx.h"

WarHandler::WarHandler()
{
	RegWorldEvent(ID_NetEntityAttackNotify, &WarHandler::onNetEntityAttackNotify, this);
	RegWorldEvent(ID_NetEntityFollowNotify, &WarHandler::onNetEntityFollowNotify, this);
	RegWorldEvent(ID_NetEntityCancelFollowNotify, &WarHandler::onNetEntityCancelFollowNotify, this);
}

int32 WarHandler::onNetEntityAttackNotify(Player* player, NetEntityAttackNotify* nfy)
{
	War* war = GetModule(WarModule)->getWar(player->getGuid());
	if (war == NULL)
		return 0;
	nfy->guid = player->getGuid();
	if (!war->useSkill(nfy->index))
		return 0;
	player->sendPacketToView(*nfy);
	return 0;
}

int32 WarHandler::onNetEntityFollowNotify(Player* player, NetEntityFollowNotify* nfy)
{
	Entity* tar = sWorld.FindEntByGuid(nfy->tarGuid);
	if (tar == NULL)
		return 0;
	
	GetModule(WarModule)->fllowTarget(player, tar);
	return 0;
}

int32 WarHandler::onNetEntityCancelFollowNotify(Player* player, NetEntityCancelFollowNotify* nfy)
{
	GetModule(WarModule)->fllowTarget(player, NULL);
	return 0;
}

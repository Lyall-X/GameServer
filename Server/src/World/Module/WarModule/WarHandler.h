#pragma once

class WarHandler : public Object
{
public:
	WarHandler();
protected:
	int32 onNetEntityAttackNotify(Player* player, NetEntityAttackNotify* nfy);
	int32 onNetEntityFollowNotify(Player* player, NetEntityFollowNotify* nfy);
	int32 onNetEntityCancelFollowNotify(Player* player, NetEntityCancelFollowNotify* nfy);
};
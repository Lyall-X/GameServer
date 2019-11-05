#pragma once

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();
	Player* AddPlrByAccId(Player* aPlr);
	Player* AddPlrByUserId(Player* aPlr);
	Player* FindPlrByAccId(uint32 accId);
	Player* FindPlrByUserId(uint32 userId);

	void DelPlrByAccId(uint32 accId);
	void DelPlrByUserId(uint32 userId);
protected:
	std::map<uint32, Player*>			mMapAccIdPlayer;
	std::map<uint32, Player*>			mMapUserIdPlayer;
};
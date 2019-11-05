#pragma once

class GameModule : public Module
{
	DECLARE_CLASS(GameModule)
public:
	GameModule();
	virtual ~GameModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	GameComponent* FindGameModle(uint32 insId);
	GameComponent* FindPlrGameModle(uint32 userId);
	GameComponent* FindRoomGameModle(uint32 roomId);
	GameEntity* FindPlrGameEnt(uint32 userId);

	GameComponent* AddGameModle(GameComponent* aGameModle);
	GameComponent* AddPlrGameModle(uint32 userId, GameComponent* aGameModle);
	GameComponent* AddRoomGameModle(uint32 roomId, GameComponent* aGameModle);

	void DelGameModle(uint32 insId);
	void DelPlrGameModle(uint32 userId);
	void DelRoomGameModle(uint32 roomId);
public:
	bool DoStartGame(Room* aRoom);
	bool DoCloseGame(uint32 roomId);
	bool DoOperateSee(Player* aPlr);
	bool DoOperateGiveup(Player* aPlr);
	bool DoOperateChipinReq(Player* aPlr, uint32 gold);
	bool DoOperateCallReq(Player* aPlr);
	bool DoOperateCompareReq(Player* aPlr, uint32 tarUserId);
protected:
	std::map<uint32, GameComponent*>				mMapGameModle;
	std::map<uint32, GameComponent*>				mMapRoomGameModle;
	std::map<uint32, GameComponent*>				mMapPlrGameModle;
};

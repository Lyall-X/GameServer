#pragma once

enum RankType
{
	RT_None, 
	RT_Gold,
	RT_Diamond,
};

class RankModule : public Module
{
public:
	DECLARE_CLASS(RankModule)
public:
	RankModule();
	~RankModule();
protected:

	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);
	virtual bool onLoadEnd(Player* player, Dictionary& bytes);
	virtual bool onSaveEnd(Player* player, Dictionary& bytes);
public:
	bool RankAdd(uint8 rankType, uint32 userId, uint64 value);
	bool RankFind(uint8 rankType, uint32 start, uint32 count);
protected:
	int32 onRankFind(RedisEvent& e);
};
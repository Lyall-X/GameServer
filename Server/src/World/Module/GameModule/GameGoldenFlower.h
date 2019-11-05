#pragma once

#define MAX_UNIT_POKER_COUNT 3
#define MAX_CARD_COUNT 52

enum GameState
{
	GS_None,
	GS_Normal,		// 正常
	GS_Giveup,		// 放弃
	GS_Lose,		// 比输
	GS_Win,			// 比赢
};

class GameEntity
{
public:
	GameEntity();
	~GameEntity();
	bool operator >> (GameEntityInfo& info);
public:
	uint32					userId = 0;
	uint8					maxCount = MAX_UNIT_POKER_COUNT;
	std::vector<uint8>		cards;
	std::vector<uint8>		seeCards;

	std::string ToString();
	uint32 GetCardCount() { return (uint32)cards.size(); }
	uint32 GetCard(uint32 idx);
	uint32 getGold() { return mGold; }
	uint32 GetUserId() { return userId; }
	std::vector<uint8>& GetCards() { return cards; }
	std::vector<uint8>& GetSeeCards() { return seeCards; }
	GameState GetState() { return (GameState)mState; }
	//luabind::object GetCards();
	ThreeCardType GetThreeCardType();

	void addGold(uint32 gold) { mGold += gold; }
	void SetState(GameState gState) { mState = gState; }
	void SetIsSee(bool isSee) { mIsSee = isSee; }

	bool GetIsSee() { return mIsSee; }
protected:
	uint32					mGold;
	uint8					mState;
	bool					mIsSee;					// 是否看
};

/*
* 
*/
class GameGoldenFlower : public GameComponent
{
	friend class GameModule;
public:
	GameGoldenFlower();
	~GameGoldenFlower();
	bool operator >> (GameGoldenFlowerInfo& info);

	void DoShuffle();			// 洗
	void DoCutCard();			// 切
	void SetBanker(uint32 userId) { mBankerUserId = userId; }
	void SetCurSpeakPlr(uint32 userId);
	void SetSpeakTime(uint32 speakTime) { mSpeakTime = speakTime; }
	void SetSpeakStartTime(uint32 t);
	void SetGameLv(uint8 gamelv) { mGameLv = gamelv; }
	void SetCurMaxGold(uint32 maxGold) { mCurMaxGold = maxGold; }
	void SetRound(uint32 round) { mRound = round; }
	void SetCurGold(uint32 gold) { mCurGold = gold; }
	void AddCurGold(uint32 gold) { mCurGold += gold; }
	void IncSpeakCount();

	bool IsMaxRount();
	uint8 DoDealPoker();		// 发

	uint32 GetNextSpeakPlr();
	uint32 GetBanker() { return mBankerUserId; }
	uint32 GetCurSpeak() { return mCurSpeakUserId; }
	uint32 GetSpeakTime() { return mSpeakTime; }
	uint32 GetSpeakStartTime() { return mSpeakStartTime; }
	uint32 GetCurGold() { return mCurGold; }
	uint32 GetCurMaxGold();
	uint32 GetRound() { return mRound; }
	uint32 GetSpeakCount() { return mSpeakCount; }
	uint32 CalculateUseGold(uint32 userGold);
	uint32 GetLivePlrCount();
	uint8 GetGameLv() { return mGameLv; }

	uint32 CheckUserGold(uint32 gold);
	virtual std::string ToString();

	int32 CheckCardsToTarget(uint32 srcUserId, uint32 dstUserId);
	bool DoUseGold(Player* aPlr, uint32 gold);
	bool DoNext();
	bool DoResult(uint32& winer);
protected:
	virtual bool OnStart();
	virtual bool OnClose();
	virtual bool OnEnter(GameEntity* aGameEnt);
	virtual bool OnLeave(GameEntity* aGameEnt);

	bool OnSeeCard(GameEntity* aGameEnt);
	bool OnGiveup(GameEntity* aGameEnt);
	bool OnChipinReq(GameEntity* aGameEnt, uint32 gold);
	bool OnCallReq(GameEntity* aGameEnt, uint32 gold);
	bool OnCompareReq(GameEntity* aGameEnt, uint32 tarUserId,uint8 result);

protected:
	int32 onSpeakTime(TimerEvent& e);
protected:
	uint8							mCards[MAX_CARD_COUNT];
	std::queue<uint8>				mCurCards;				// 没有使用
	std::queue<uint8>				mOpenCards;				// 已经使用
	uint32							mBankerUserId;			// 庄家
	uint32							mCurSpeakUserId;		// 当前说话的人
	uint32							mSpeakTime;				// 说话剩余时间秒
	uint32							mSpeakStartTime;		// 开始说话时间
	uint32							mCurGold;
	uint8							mGameLv;
	uint32							mCurMaxGold;			// 当前最大
	uint32							mRound;					// 回合数
	uint32							mSpeakCount;
	uint32							mSpeakTimeEvent;
};
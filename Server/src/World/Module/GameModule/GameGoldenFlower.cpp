#include "stdafx.h"

GameGoldenFlower::GameGoldenFlower()
{
	mBankerUserId = 0;
	mCurSpeakUserId = 0;
	mSpeakTime = 0;
	mSpeakStartTime = 0;
	mCurGold = 0;
	mGameLv = 0;
	mCurMaxGold = 0;
	mRound = 0;
	mSpeakCount = 0;
	mSpeakTimeEvent = 0;
}

GameGoldenFlower::~GameGoldenFlower()
{

}

bool GameGoldenFlower::operator >> (GameGoldenFlowerInfo& info)
{
	info.insId = GetInsId();
	info.roomId = GetRoomId();
	info.bankerUserId = GetBanker();
	info.curSpeakUserId = GetCurSpeak();
	info.speakTime = GetSpeakTime();
	for (auto& itr : mLstGameEntity)
	{
		GameEntity* aGameEnt = itr.second;
		GameEntityInfo entInfo;
		*aGameEnt >> entInfo;
		info.gameEntInfos.push_back(entInfo);
	}
	return true;
}

void GameGoldenFlower::DoShuffle()
{
	for (uint32 i = 0; i < MAX_CARD_COUNT; ++i) {
		mCards[i] = i + 1;
	}
	uint32 curSize = MAX_CARD_COUNT;
	while (curSize > 0)
	{
		uint32 idx = rand() % curSize;
		mCurCards.push(mCards[idx]);
		curSize--;
		mCards[idx] = mCards[curSize];
	}
}

void GameGoldenFlower::IncSpeakCount()
{
	mSpeakCount++;
	if (mSpeakCount > GetGameEntCount()) {
		mRound++;
		mSpeakCount = 0;
	}
}

bool GameGoldenFlower::IsMaxRount()
{
	if (mSpeakCount + 1 > GetGameEntCount()) {
		GameLevelJson* aGameLvJson = sCfgMgr.getGameLevelJson(GetGameLv());
		if (aGameLvJson == NULL){
			return false;
		}

		if (GetRound() + 1 >= aGameLvJson->Maxround) {
			return true;
		}
	}
	return false;
}

uint8 GameGoldenFlower::DoDealPoker()
{
	uint8 pkr = mCurCards.front();
	mCurCards.pop();
	return pkr;
}

void GameGoldenFlower::DoCutCard()
{

}

void GameGoldenFlower::SetCurSpeakPlr(uint32 userId)
{
	mCurSpeakUserId = userId;
	mTimer.removeEventListener(mSpeakTimeEvent);
}

void GameGoldenFlower::SetSpeakStartTime(uint32 t)
{
	 mSpeakStartTime = t; 
	 mSpeakTimeEvent = mTimer.addEventListener((EventCallback)&GameGoldenFlower::onSpeakTime, this, mSpeakStartTime + 5.f);
}

uint32 GameGoldenFlower::GetNextSpeakPlr()
{
	int32 idx = GetPlrInx(mCurSpeakUserId);
	for (int8 i = 0; i < GetRoom()->GetMaxCount() - 1; ++i)
	{
		idx = idx + 1 >= GetRoom()->GetMaxCount() ? 0 : idx + 1;
		GameEntity* aGameEnt = GetGameEnt(idx);
		if (aGameEnt == NULL) continue;
		if (aGameEnt->GetState() == GS_Normal)
			return aGameEnt->userId;
	}
	return 0;
}

uint32 GameGoldenFlower::GetCurMaxGold()
{
	uint32 curMaxGold = mCurMaxGold;
	if (curMaxGold != 0) return curMaxGold;

	GameLevelJson* aGameLvJson = sCfgMgr.getGameLevelJson(GetGameLv());
	if (aGameLvJson == NULL) {
		return false;
	}
	curMaxGold = curMaxGold == 0 ? aGameLvJson->Mingold : curMaxGold;
	SetCurMaxGold(curMaxGold);
	return curMaxGold;
}

uint32 GameGoldenFlower::CalculateUseGold(uint32 userGold)
{
	GameLevelJson* aGameLvJson = sCfgMgr.getGameLevelJson(GetGameLv());
	if (aGameLvJson == NULL){
		return 0;
	}

	uint32 curMaxGold = GetCurMaxGold();
	curMaxGold = curMaxGold == 0 ? aGameLvJson->Mingold : curMaxGold;
	userGold = CheckUserGold(userGold);
	if (userGold < curMaxGold) {
		return 0;
	}
	return userGold;
}

uint32 GameGoldenFlower::GetLivePlrCount()
{
	uint32 num = 0;
	for (auto& itr : mLstGameEntity)
	{
		GameEntity* aGameEnt = itr.second;
		if (aGameEnt->GetState() == GS_Normal)
			num++;
	}
	return num;
}

uint32 GameGoldenFlower::CheckUserGold(uint32 gold)
{
	GameLevelJson* aGameJson = sCfgMgr.getGameLevelJson(GetGameLv());
	if (aGameJson == NULL) return 0;

	gold = gold > aGameJson->Maxgold ? aGameJson->Maxgold : gold;
	gold = gold < aGameJson->Mingold ? aGameJson->Mingold : gold;
	return gold;
}

std::string GameGoldenFlower::ToString()
{
	std::string str;
	char szBuffer[256] = { 0 };
	sprintf_s(szBuffer, 256, "ID:%d rID:%d Banker:%d sID:%d sTime:%ds\n", GetInsId(), GetRoomId(), GetBanker(), GetCurSpeak(), GetSpeakTime());
	str += szBuffer;
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		str += mLstGameEntity[i]->ToString();
		str += "\n";
	}

	return str;
}

int32 GameGoldenFlower::CheckCardsToTarget(uint32 srcUserId, uint32 dstUserId)
{
	return LuaEngine::Call(sScriptCard, "ComparePlrCard", srcUserId, dstUserId);
}

bool GameGoldenFlower::DoUseGold(Player* aPlr, uint32 gold)
{
	GameEntity* aGameEnt = FindGameEnt(aPlr->getUserId());
	if (aGameEnt == NULL) return false;
	sProperty.addGold(aPlr, -(int32)gold);
	aGameEnt->addGold(gold);
	AddCurGold(gold);
	SetCurMaxGold(gold);
	IncSpeakCount();
	DoNext();
	return true;
}

bool GameGoldenFlower::DoNext()
{
	uint32 nextUserId = GetNextSpeakPlr();
	if (nextUserId == 0)
	{
		return false;
	}
	SetCurSpeakPlr(nextUserId);
	SetSpeakStartTime(DateTime::Now());
	return true;
}

bool GameGoldenFlower::DoResult(uint32& winer)
{
	if (GetLivePlrCount() == 1)
	{
		uint32 num = 0;
		for (auto& itr : mLstGameEntity) {
			GameEntity* aGameEnt = itr.second;
			if (aGameEnt->GetState() == GS_Normal) {
				winer = aGameEnt->GetUserId();

				Player* aPlr = sWorld.FindPlrByUserId(winer);
				if (aPlr) {
					sProperty.addGold(aPlr, GetCurGold());
				}
				break;
			}
		}

		return true;
	}
	return false;
}

bool GameGoldenFlower::OnStart()
{
	LuaEngine::Call(sScriptGame, "OnStart", GetInsId());
	return true;
}

bool GameGoldenFlower::OnClose()
{
	LuaEngine::Call(sScriptGame, "OnClose", GetInsId());
	return true;
}

bool GameGoldenFlower::OnEnter(GameEntity* aGameEnt)
{
	LuaEngine::Call(sScriptGame, "OnEnter", GetInsId(), aGameEnt->userId);
	return true;
}

bool GameGoldenFlower::OnLeave(GameEntity* aGameEnt)
{
	LuaEngine::Call(sScriptGame, "OnLeave", GetInsId(), aGameEnt->userId);
	return true;
}

bool GameGoldenFlower::OnSeeCard(GameEntity* aGameEnt)
{
	LuaEngine::Call(this, sScriptGame, "OnSeeCard", aGameEnt->userId);
	return true;
}

bool GameGoldenFlower::OnGiveup(GameEntity* aGameEnt)
{
	LuaEngine::Call(this, sScriptGame, "OnGiveup", aGameEnt->userId);
	return true;
}

bool GameGoldenFlower::OnChipinReq(GameEntity* aGameEnt, uint32 gold)
{
	LuaEngine::Call(this, sScriptGame, "OnChipinReq", aGameEnt->userId, gold);
	return true;
}

bool GameGoldenFlower::OnCallReq(GameEntity* aGameEnt, uint32 gold)
{
	LuaEngine::Call(this, sScriptGame, "OnCallReq", aGameEnt->userId, gold);
	return true;
}

bool GameGoldenFlower::OnCompareReq(GameEntity* aGameEnt, uint32 tarUserId, uint8 result)
{
	LuaEngine::Call(this, sScriptGame, "OnCompareReq", aGameEnt->userId, tarUserId, result);
	return true;
}

int32 GameGoldenFlower::onSpeakTime(TimerEvent& e)
{
	GameEntity* aGameEnt = FindGameEnt(mCurSpeakUserId);
	if (aGameEnt == NULL) return 0;

	aGameEnt->SetState(GS_Giveup);
	DoNext();
	Room* aRoom = GetRoom();
	if (aRoom == NULL) return 0;

	uint32 winer = 0;
	if (DoResult(winer))
	{
		NetGameCloseNotify nfy;
		nfy.winUserId = winer;
		nfy.winGold = GetCurGold();
		aRoom->sendPacketToAll(nfy);

		sGame.DoCloseGame(GetRoomId());
	}
	return 0;
}

GameEntity::GameEntity()
{

}

GameEntity::~GameEntity()
{

}

bool GameEntity::operator >> (GameEntityInfo& info)
{
	info.userId = userId;
	info.cards = GetSeeCards();
	info.userGold = getGold();
	return true;
}

std::string GameEntity::ToString()
{
	char szBuff[256] = { 0 };
	sprintf_s(szBuff, 256, "[%d]", userId);
	uint32 strLen = (uint32)strlen(szBuff);
	for (uint32 i = 0; i < cards.size(); ++i)
	{
		sprintf_s(szBuff + strLen, 256 - strLen, "%d ", cards[i]);
		strLen = (uint32)strlen(szBuff);
	}
	return szBuff;
}

uint32 GameEntity::GetCard(uint32 idx)
{
	if (idx >= cards.size()) return 0;
	return cards[idx];
}

//luabind::object GameEntity::GetCards()
//{
//	LuaScript* luaScript = sLua.getScript(sScriptPlayer);
//	if (luaScript == NULL) return luabind::object();
//
//	luabind::object obj = luabind::newtable(luaScript->getLuaState());
//	for (uint32 i = 0; i < cards.size(); ++i) {
//		obj[i] = cards[i];
//	}
//	return obj;
//}

ThreeCardType GameEntity::GetThreeCardType()
{
	return ThreeCardType::TCT_None;
}

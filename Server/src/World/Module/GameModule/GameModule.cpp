#include "stdafx.h"

IMPLEMENT_CLASS(GameModule, Module)

GameModule::GameModule()
{

}

GameModule::~GameModule()
{

}

bool GameModule::Initialize()
{
	return true;
}

bool GameModule::Update(float time, float delay)
{
	std::map<uint32, GameComponent*> mapGame = mMapGameModle;

	for (auto& itr : mapGame)
	{
		itr.second->Update(time, delay);
	}

	return true;
}

bool GameModule::Destroy()
{
	return true;
}

bool GameModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool GameModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

GameComponent* GameModule::AddGameModle(GameComponent* aGameModle)
{
	auto itr = mMapGameModle.find(aGameModle->GetInsId());
	if (itr != mMapGameModle.end())
		return NULL;
	mMapGameModle[aGameModle->GetInsId()] = aGameModle;
	return aGameModle;
}

GameComponent* GameModule::FindGameModle(uint32 insId)
{
	auto itr = mMapGameModle.find(insId);
	if (itr != mMapGameModle.end())
		return itr->second;
	return NULL;
}

void GameModule::DelGameModle(uint32 insId)
{
	auto itr = mMapGameModle.find(insId);
	if (itr != mMapGameModle.end()){
		delete itr->second;
		mMapGameModle.erase(itr);
	}
}

GameComponent* GameModule::AddPlrGameModle(uint32 userId, GameComponent* aGameModle)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		return NULL;

	mMapPlrGameModle[userId] = aGameModle;
	return aGameModle;
}

GameComponent* GameModule::AddRoomGameModle(uint32 roomId, GameComponent* aGameModle)
{
	auto itr = mMapRoomGameModle.find(roomId);
	if (itr != mMapRoomGameModle.end())
		return NULL;

	mMapRoomGameModle[roomId] = aGameModle;
	return aGameModle;
}

GameComponent* GameModule::FindPlrGameModle(uint32 userId)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		return itr->second;
	return NULL;
}

void GameModule::DelPlrGameModle(uint32 userId)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		mMapPlrGameModle.erase(itr);
}

void GameModule::DelRoomGameModle(uint32 roomId)
{
	auto itr = mMapRoomGameModle.find(roomId);
	if (itr != mMapRoomGameModle.end())
		mMapRoomGameModle.erase(itr);
}

GameComponent* GameModule::FindRoomGameModle(uint32 roomId)
{
	auto itr = mMapRoomGameModle.find(roomId);
	if (itr != mMapRoomGameModle.end())
		return itr->second;
	return NULL;
}

GameEntity* GameModule::FindPlrGameEnt(uint32 userId)
{
	GameComponent* aGameModule = FindPlrGameModle(userId);
	if (aGameModule == NULL)
		return NULL;
	return aGameModule->FindGameEnt(userId);
}

bool GameModule::DoStartGame(Room* aRoom)
{
	if (FindRoomGameModle(aRoom->GetInsId()))
	{
		// 当前房间在游戏中
		return false;
	}

	GameGoldenFlower* aGame = new GameGoldenFlower;
	AddGameModle(aGame);

	aGame->DoShuffle();
	aGame->SetRoomId(aRoom->GetInsId());
	aGame->SetGameLv(1);
	aGame->SetBanker(aRoom->GetMaster()->mUserId);
	aGame->SetCurSpeakPlr(aGame->GetBanker());
	aGame->SetSpeakTime(sSpeakTotalTime);
	aGame->SetSpeakStartTime(DateTime::Now());

	aRoom->SetGameInsId(aGame->GetInsId());
	aRoom->DoAllStart();


	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer == NULL) continue;
		if (aRoomPlayer->GetState() != RPS_Game) continue;
		
		GameEntity* aGameEnt = new GameEntity;
		aGameEnt->userId = aRoomPlayer->mUserId;
		aGame->AddGameEnt(aGameEnt);
		AddPlrGameModle(aRoomPlayer->mUserId, aGame);
	}

	for (uint8 k = 0; k < MAX_UNIT_POKER_COUNT; ++k)
	{
		for (uint32 i = 0; i < aGame->GetGameEntCount(); ++i)
		{
			GameEntity* aGameEnt = aGame->GetGameEnt(i);
			aGameEnt->cards.push_back(aGame->DoDealPoker());
			aGameEnt->seeCards.push_back(0);
		}
	}

	aGame->OnStart();
	for (uint32 i = 0; i < aGame->GetGameEntCount(); ++i)
	{
		GameEntity* aGameEnt = aGame->GetGameEnt(i);
		if (aGameEnt)
		{
			aGame->OnEnter(aGameEnt);
		}
	}

	NetGameStartNotify nfy;
	*aGame >> nfy.info;
	aRoom->sendPacketToAll(nfy);
	return true;
}

bool GameModule::DoCloseGame(uint32 roomId)
{
	Room* aRoom = sRoom.FindRoom(roomId);
	if (aRoom == NULL)
	{
		return false;
	}

	for (int32 i = 0 ;i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer)
		{
			aRoomPlayer->SetState(RPS_None);
		}
	}
	NetRoomInfoNotify nfy;
	(*aRoom) >> nfy.roomInfo;
	aRoom->sendPacketToAll(nfy);

	GameComponent* aGameModle = FindRoomGameModle(roomId);
	if (aGameModle == NULL)
	{
		LOG_ERROR("%d 房间没有开始游戏", roomId);
		return false;
	}
	std::queue<uint32> delQueue;
	for (uint32 i = 0; i < aGameModle->GetGameEntCount(); ++i)
	{
		GameEntity* aGameEnt = aGameModle->GetGameEnt(i);
		if (aGameEnt)
		{
			aGameModle->OnLeave(aGameEnt);
			delQueue.push(aGameEnt->userId);
		}
	}

	aGameModle->OnClose();

	while (delQueue.size())
	{
		uint32 userId = delQueue.front();
		delQueue.pop();

		DelPlrGameModle(userId);
		aGameModle->DelGameEnt(userId);
	}
	DelRoomGameModle(roomId);
	DelGameModle(aGameModle->GetInsId());
	return true;
}


bool GameModule::DoOperateSee(Player* aPlr)
{
	NetGameOperateSeeRes res;

	GameEntity* aGameEnt = FindPlrGameEnt(aPlr->getUserId());
	if (aGameEnt == NULL)
	{
		res.result = NResultFail;
		aPlr->sendPacket(res);
		return false;
	}

	if (aGameEnt->GetIsSee())
	{
		res.result = NResultFail;
		aPlr->sendPacket(res);
		return false;
	}
	aGameEnt->SetIsSee(true);
	aGameEnt->GetSeeCards() = aGameEnt->GetCards();
	res.result = NResultSuccess;
	res.cards = aGameEnt->GetCards();
	GameGoldenFlower* aGame = (GameGoldenFlower*)FindPlrGameModle(aPlr->getUserId());
	if (aGame) {
		aGame->OnSeeCard(aGameEnt);
	}

	aPlr->sendPacket(res);
	return true;
}

bool GameModule::DoOperateGiveup(Player* aPlr)
{
	Room* aRoom = sRoom.FindPlayerRoom(aPlr->getUserId());
	if (aRoom == NULL) return false;

	NetGameOperateGiveupRes res;
	res.userId = aPlr->getUserId();
	GameEntity* aGameEnt = FindPlrGameEnt(aPlr->getUserId());
	if (aGameEnt == NULL)
	{
		aPlr->sendPacket(res);
		return false;
	}
	if (aGameEnt->GetState() != GS_Normal)
	{
		aPlr->sendPacket(res);
		return false;
	}
	aGameEnt->SetState(GS_Giveup);
	GameGoldenFlower* aGame = (GameGoldenFlower*)FindPlrGameModle(aPlr->getUserId());
	if (aGame == NULL) return false;
	aGame->OnGiveup(aGameEnt);
	if (aGame->GetCurSpeak() == aPlr->getUserId())
	{
		aGame->DoNext();
	}
	
	aRoom->sendPacketToAll(res);

	uint32 winer = 0;
	if (aGame->DoResult(winer))
	{
		NetGameCloseNotify nfy;
		nfy.winUserId = winer;
		nfy.winGold = aGame->GetCurGold();
		aRoom->sendPacketToAll(nfy);

		DoCloseGame(aGame->GetRoomId());
	}
	return true;
}

bool GameModule::DoOperateChipinReq(Player* aPlr, uint32 gold)
{
	GameGoldenFlower* aGame = (GameGoldenFlower*)FindPlrGameModle(aPlr->getUserId());
	if (aGame == NULL)
	{
		return false;
	}
	Room* aRoom = sRoom.FindRoom(aGame->GetRoomId());
	if (aRoom == NULL)
	{
		return false;
	}
	if (aGame->GetCurSpeak() != aPlr->getUserId())
	{
		return false;
	}

	GameEntity* aGameEnt = aGame->GetGameEnt(aPlr->getUserId());
	if (aGameEnt == NULL)
	{
		return false;
	}

	if (aGame->IsMaxRount())
	{
		// 最大回合
		return false;
	}
	uint32 userGold = aGame->CalculateUseGold(gold);
	if (userGold == 0)
	{
		return false;
	}
	NetGameOperateChipinRes res;
	if (sProperty.hasGold(aPlr, userGold))
	{
		aGame->DoUseGold(aPlr, userGold);

		if (aGame) {
			aGame->OnChipinReq(aGameEnt, userGold);
		}

		res.result = NResultSuccess;
		res.userId = aPlr->getUserId();
		res.gold = userGold;
		res.state =(uint8) aGameEnt->GetState();
		res.nextSpeakUserId = aGame->GetCurSpeak();
		res.speakTime = aGame->GetSpeakTime();
		aRoom->sendPacketToAll(res);
		uint32 winer = 0;
		if (aGame->DoResult(winer)) {
			NetGameCloseNotify nfy;
			nfy.winUserId = winer;
			nfy.winGold = aGame->GetCurGold();
			aRoom->sendPacketToAll(nfy);

			DoCloseGame(aGame->GetRoomId());
		}
		return true;
	}
	res.result = NResultFail;
	res.state = (uint8)aGameEnt->GetState();
	aRoom->sendPacketToAll(res);
	return false;
}

bool GameModule::DoOperateCallReq(Player* aPlr)
{
	GameGoldenFlower* aGame = (GameGoldenFlower*)FindPlrGameModle(aPlr->getUserId());
	if (aGame == NULL)
	{
		return false;
	}
	Room* aRoom = sRoom.FindRoom(aGame->GetRoomId());
	if (aRoom == NULL)
	{
		return false;
	}
	if (aGame->GetCurSpeak() != aPlr->getUserId())
	{
		return false;
	}

	GameEntity* aGameEnt = aGame->GetGameEnt(aPlr->getUserId());
	if (aGameEnt == NULL)
	{
		return false;
	}
	if (aGame->IsMaxRount())
	{
		// 最大回合
		return false;
	}

	GameLevelJson* aGameLvJson = sCfgMgr.getGameLevelJson(aGame->GetGameLv());
	if (aGameLvJson == NULL)
	{
		return false;
	}

	uint32 userGold = aGame->CalculateUseGold(aGame->GetCurMaxGold());
	if (userGold == 0)
	{
		return false;
	}
	NetGameOperateCallRes res;
	if (sProperty.hasGold(aPlr, userGold))
	{
		aGame->DoUseGold(aPlr, userGold);
		aGame->OnCallReq(aGameEnt, userGold);

		res.result = NResultSuccess;
		res.userId = aPlr->getUserId();
		res.gold = userGold;
		res.state = (uint8)aGameEnt->GetState();
		res.nextSpeakUserId = aGame->GetCurSpeak();
		res.speakTime = aGame->GetSpeakTime();
		aRoom->sendPacketToAll(res);
		uint32 winer = 0;
		if (aGame->DoResult(winer)) {
			NetGameCloseNotify nfy;
			nfy.winUserId = winer;
			nfy.winGold = aGame->GetCurGold();
			aRoom->sendPacketToAll(nfy);

			DoCloseGame(aGame->GetRoomId());
		}

		return true;
	}
	res.result = NResultFail;
	res.state = (uint8)aGameEnt->GetState();
	aRoom->sendPacketToAll(res);
	return false;
}

bool GameModule::DoOperateCompareReq(Player* aPlr, uint32 tarUserId)
{
	GameGoldenFlower* aGame = (GameGoldenFlower*)FindPlrGameModle(aPlr->getUserId());
	if (aGame == NULL) {
		return false;
	}
	Room* aRoom = sRoom.FindRoom(aGame->GetRoomId());
	if (aRoom == NULL) {
		return false;
	}

	GameEntity* aGameEnt = FindPlrGameEnt(aPlr->getUserId());
	if (aGameEnt == NULL) {
		return false;
	}
	GameEntity* tarGameEnt = FindPlrGameEnt(tarUserId);
	if (tarGameEnt == NULL) {
		return false;
	}

	if (aGame->IsMaxRount()) {
		// 最大回合
		return false;
	}

	NetGameOperateCompareRes res;

	uint32 userGold = aGame->GetCurMaxGold();
	if (userGold == 0)
	{
		return false;
	}
	if (sProperty.hasGold(aPlr, userGold))
	{
		int32 ret = aGame->CheckCardsToTarget(aPlr->getUserId(), tarUserId);
		if (ret == VST_Win) {
			tarGameEnt->SetState(GS_Lose);
		}
		else {
			aGameEnt->SetState(GS_Lose);
		}

		aGame->DoUseGold(aPlr, userGold);

		aGame->OnCompareReq(aGameEnt, tarUserId, ret);

		res.result = NResultSuccess;
		res.userId = aPlr->getUserId();
		res.gold = userGold;
		res.state = (uint8)aGameEnt->GetState();

		res.tarUserId = tarUserId;
		res.tarState = tarGameEnt->GetState();

		res.nextSpeakUserId = aGame->GetCurSpeak();
		res.speakTime = aGame->GetSpeakTime();
		aRoom->sendPacketToAll(res);

		uint32 winer = 0;
		if (aGame->DoResult(winer)) {
			NetGameCloseNotify nfy;
			nfy.winUserId = winer;
			nfy.winGold = aGame->GetCurGold();
			aRoom->sendPacketToAll(nfy);

			DoCloseGame(aGame->GetRoomId());
		}
		return true;
	}
	res.result = NResultFail;
	res.state = (uint8)aGameEnt->GetState();
	aRoom->sendPacketToAll(res);
	return false;
}

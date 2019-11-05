#include "stdafx.h"

IMPLEMENT_CLASS(FriendsModule, Module);

FriendsModule::FriendsModule()
{

}

FriendsModule::~FriendsModule()
{
}

Friends* FriendsModule::AddFriends(Friends* frds)
{
	if (GetFriends(frds->GetUserId())) return NULL;

	mMapFriends.insert(std::make_pair(frds->GetUserId(), frds));
	return frds;
}

Friend* FriendsModule::AddFriend(uint32 tarUserId, Player* tar)
{
	if (tarUserId == tar->getUserId()) return false;

	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL)
	{
		frds = new Friends(tarUserId);
		if (AddFriends(frds) == NULL) {
			delete  frds;
			frds = NULL;
		}
	}
	if (frds == NULL) return NULL;
	return  frds->AddFriend(tar);
}

Friend* FriendsModule::AddFriend(uint32 tarUserId, Friend* frdTar)
{
	if (tarUserId == frdTar->mUserId) return false;

	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL)
	{
		frds = new Friends(tarUserId);
		if (AddFriends(frds) == NULL) {
			delete  frds;
			frds = NULL;
		}
	}

	if (frds == NULL) return NULL;
	return  frds->AddFriend(frdTar);
}

Friend* FriendsModule::FindFriend(uint32 tarUserId, uint32 frdUserId)
{
	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL) return NULL;
	return frds->FindFriend(frdUserId);
}

bool FriendsModule::DelFriend(uint32 tarUserId, uint32 frdUserId)
{
	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL)
		return false;
	return frds->DelFriend(frdUserId);
}

bool FriendsModule::DelFriends(uint32 tarUserId)
{
	auto itr = mMapFriends.find(tarUserId);
	if (itr == mMapFriends.end()) return false;
	mMapFriends.erase(itr);
	return true;
}

Friends* FriendsModule::GetFriends(uint32 userId)
{
	auto itr = mMapFriends.find(userId);
	if (itr == mMapFriends.end()) return NULL;

	return itr->second;
}

bool FriendsModule::MutualBindFriend(Player* tar, Player* frd)
{
	if (FindFriend(tar->getUserId(), frd->getUserId()))
	{
		return false;
	}
	if (FindFriend(frd->getUserId(), tar->getUserId()))
	{
		return false;
	}

	if (tar->getUserId() == frd->getUserId()) return false;
	if (!AddFriend(tar->getUserId(), frd))
		return false;
	return AddFriend(frd->getUserId(), tar) != NULL;
}

bool FriendsModule::MutualDebindFriend(uint32 tarUserId, uint32 frdUserId)
{
	if (tarUserId == frdUserId) return false;
	DelFriend(tarUserId, frdUserId);
	DelFriend(frdUserId, tarUserId);
	return true;
}

void FriendsModule::ClearFriends()
{
	for (auto itr : mMapFriends)
	{
		delete itr.second;
	}
	mMapFriends.clear();
}

void FriendsModule::DoFriendsList(Player* aPlr)
{
	NetFriendListRes res;
	Friends* frds = GetFriends(aPlr->getUserId());
	if (frds == NULL)
	{
		aPlr->sendPacket(res);
		return;
	}
	for (auto& itr : frds->GetGroups())
	{
		res.gruopNames.push_back(itr.first);
		res.gruopIds.push_back(itr.second);
	}
	for (Friend* frd : frds->GetFriends())
	{
		PlayerRecord* aPlrRecrd = sWorld.FindPlrRecord(frd->mUserId);
		if (aPlrRecrd)
		{
			FriendInfo info;
			info.userId = aPlrRecrd->GetUserId();
			info.name = aPlrRecrd->GetName();
			info.state = aPlrRecrd->GetOnline();
			info.groupId = frd->mGroupId;
			res.friendInfos.push_back(info);
		}
	}
	aPlr->sendPacket(res);
}

bool FriendsModule::Initialize()
{
	sRedisProxy.addEventListener("OnRedisAuth", (EventCallback)&FriendsModule::onRedisAuth, this);
	//sWorld.addEventListener("OnChangeName", (EventCallback)&FriendsModule::onChangeName, this);
	return true;
}

bool FriendsModule::Update(float time, float delay)
{
	return true;
}

bool FriendsModule::Destroy()
{
	return true;
}

bool FriendsModule::onEnterWorld(Player* player, Dictionary& dict)
{	
	return true;
}

bool FriendsModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool FriendsModule::onLoad(Player* player, Dictionary& bytes)
{
	Json::Value& root = player->GetJson();
	Json::Value arrayVal = root["friends"];

	for (uint32 i = 0 ; i < arrayVal.size(); ++i)
	{
		Friend* frd = new Friend;
		(*frd) << arrayVal[i];
		if (!AddFriend(player->getUserId(), frd)) {
			delete frd;
		}
	}
	
	return true;
}

bool FriendsModule::onSave(Player* player, Dictionary& bytes)
{
	Friends* frds = GetFriends(player->getUserId());
	if (frds == NULL) return false;

	Json::Value& root = player->GetJson();

	Json::Value arrayVal;
	for (Friend* frd : frds->GetFriends())
	{
		Json::Value frdJson;
		(*frd) >> frdJson;
		arrayVal.append(frdJson);
	}
	root["friends"] = arrayVal;
	if (player->CanDestroy()) {
		DelFriends(player->getUserId());
	}
	return true;
}


int32 FriendsModule::onRedisAuth(Event& e)
{
	//char szBuffer[64] = { 0 };
	//sprintf_s(szBuffer, 64, "hgetall %s", sUser);
	//float64 s0 = DateTime::GetNowAppUS();
	//sRedisProxy.sendCmd(szBuffer, (EventCallback)&FriendsModule::onRedisAllPlr, this);

	return 0;
}

int32 FriendsModule::onChangeName(Event& e)
{
	return 0;
}

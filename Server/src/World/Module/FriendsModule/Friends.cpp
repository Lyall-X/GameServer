#include "stdafx.h"


bool Friend::operator >> (Json::Value& json)
{
	json["mUserId"] = mUserId;
	json["mGroupId"] = mGroupId;
	return true;
}

bool Friend::operator << (Json::Value& json)
{
	mUserId = json["mUserId"].asUInt();
	mGroupId = json["mGroupId"].asUInt();
	return true;
}

Friends::Friends(uint32 tarUserId):
mUserId(tarUserId)
{
	mGroups["ÎÒµÄºÃÓÑ"] = 0;
}

Friends::~Friends()
{
	ClearFriends();
}

Friend* Friends::AddFriend(Player* tar)
{
	if (FindFriend(tar->getUserId()))
		return NULL;
	Friend* frd = new Friend;
	frd->mUserId = tar->getUserId();
	mFriends.push_back(frd);
	return frd;
}

Friend* Friends::AddFriend(Friend* frd)
{
	if (FindFriend(frd->mUserId)) {
		return NULL;
	}
	mFriends.push_back(frd);
	return frd;
}

bool Friends::DelFriend(uint32 userId)
{
	for (Friend* frd : mFriends)
	{
		if (frd->mUserId == userId)
		{
			delete frd;
			mFriends.remove(frd);
			return true;
		}
	}
	return false;
}

Friend* Friends::FindFriend(uint32 userId)
{
	for (Friend* frd : mFriends)
	{
		if (userId == frd->mUserId)
			return frd;
	}
	return NULL;
}

Friend* Friends::GetFriend(uint32 idx)
{
	uint32 i = 0;
	for (Friend* frd : mFriends)
	{
		if (idx == i) return frd;
		i++;
	}
	return NULL;
}

sstring Friends::GetGroupName(uint32 groupId)
{
	for (auto& itr : mGroups) {
		if (itr.second == groupId) {
			return itr.first;
		}
	}
	return "";
}

void Friends::ClearFriends()
{
	for (Friend* frd : mFriends)
		delete frd;
	mFriends.clear();
}

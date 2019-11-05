#pragma once


class Friend
{
public:
	bool operator >> (Json::Value& json);
	bool operator << (Json::Value& json);
public:
	uint32			mUserId = 0;
	uint32			mGroupId = 0;
};

class Friends
{
public:
	Friends(uint32 tarUserId);
	~Friends();
	void SetUserId(uint32 userId) { mUserId = userId; }
	void ClearFriends();
	bool DelFriend(uint32 userId);

	Friend* AddFriend(Player* tar);
	Friend* AddFriend(Friend* frd);
	Friend* FindFriend(uint32 userId);
	Friend* GetFriend(uint32 idx);
	std::list<Friend*> GetFriends() { return mFriends; }

	uint32 GetUserId() { return mUserId; }
	uint32 GetFriendsCount() { return (uint32)mFriends.size(); }
	uint32 GetGroupsCount() { return (uint32)mGroups.size(); }

	std::map<std::string, uint32>& GetGroups() { return mGroups; }
	sstring GetGroupName(uint32 groupId);
protected:
	uint32								mUserId;
	std::list<Friend*>					mFriends;
	std::map<std::string, uint32>		mGroups;
};
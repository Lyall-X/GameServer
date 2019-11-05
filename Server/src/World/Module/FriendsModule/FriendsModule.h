#pragma once

typedef std::map<uint32, Friends*> MapFriends;

class FriendsModule : public Module
{
	DECLARE_CLASS(FriendsModule)
public:
	FriendsModule();
	virtual ~FriendsModule();
public:
	Friend* AddFriend(uint32 tarUserId, Player* tar);
	Friend* AddFriend(uint32 tarUserId, Friend* frdTar);
	Friend* FindFriend(uint32 tarUserId, uint32 frdUserId);

	Friends* AddFriends(Friends* frds);
	Friends* GetFriends(uint32 userId);

	bool DelFriend(uint32 tarUserId, uint32 frdUserId);
	bool DelFriends(uint32 tarUserId);
	bool MutualBindFriend(Player* tar, Player* frd);
	bool MutualDebindFriend(uint32 tarUserId, uint32 frdUserId);

	void ClearFriends();
	void DoFriendsList(Player* aPlr);
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);

	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);
protected:
	int32 onRedisAuth(Event& e);
	int32 onChangeName(Event& e);
public:
	MapFriends					mMapFriends;
};

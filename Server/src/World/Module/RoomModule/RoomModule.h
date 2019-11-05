#pragma once

class RoomModule : public Module
{
	DECLARE_CLASS(RoomModule)
public:
	RoomModule();
	virtual ~RoomModule();

public:
	void sendPacketToRoom(Packet& packet, Player* player);

	Room* Create(Player* aPlr);
	void Remove(uint32 roomId);
	void Remove(Room* aRoom);
	bool EnterRoom(uint32 roomId, Player* aPlr, bool isMaster = false);
	bool EnterRoom(Room* aRoom, Player* aPlr, bool isMaster = false);
	bool LeaveRoom(uint32 roomId, uint32 userId);
	bool LeaveRoom(Room* aRoom, uint32 userId);
	bool ChangeMaster(Room* aRoom, uint32 oldUserId, uint32 newUserId);

	Room* FindPlayerRoom(uint32 userId);
	Room* AddPlayerRoom(uint32 userId, Room* aRoom);
	void RemovePlayerRoom(uint32 userId);

	Room* FindRoom(uint32 roomId);
	Room* AddRoom(Room* aRoom);
	void RemoveRoom(uint32 roomId);
	uint32 GetRoomCount() { return (uint32)mMapRoom.size(); }
	std::udmap<uint32, Room*>& GetMapRoom() { return mMapRoom; }
public:

	void DoCreateRoom(Player* aPlr);
	void DoEnterRoom(Player* aPlr, uint32 roomId);
	void DoLeaveRoom(Player* aPlr, uint32 roomId);
	void DoLeaveRoom(uint32 userId, uint32 roomId);
	void DoChangeRoomMaster(Player* aPlr, uint32 roomId, uint32 newUserId);
	void DoRoomList(Player* aPlr, uint32 start, uint32 count);
	void DoRoomReady(Player* aPlr, uint8 isReady);
	void DoRoomStartGame(Player* aPlr);
	void DoRoomClose(Player* aPlr);
	void DoAutoMatch(Player* aPlr);
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
protected:
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
protected:
	void ClearRoom();
	RoomMatch* FindRoomMatch(uint32 userId);
	RoomMatch* AddRoomMatch(RoomMatch& rMatch);
	void DelRoomMatch(uint32 userId);
	void MatchUpdate();
	void FreeRooms();
protected:
	std::udmap<uint32, Room*>			mMapRoom;
	std::map<uint32, Room*>				mMapPlayerRoom;
	std::list<RoomMatch>				mLstAutoMatch;
	std::set<Room*>						mSetDelRoom;
};

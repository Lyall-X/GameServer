#pragma once

class GameEntity;
class GameComponent : public Object
{
	friend class GameModule;
public:
	GameComponent();
	virtual ~GameComponent();
	virtual bool Update(float time, float delay);
public:
	GameEntity* AddGameEnt(GameEntity* aGameEnt);
	GameEntity* GetGameEnt(uint32 idx);
	GameEntity* FindGameEnt(uint32 userId);
	void DelGameEnt(uint32 userId);
	void SetRoomId(uint32 roomId) { mRoomId = roomId; }
	void SetRoom(Room* rRoom) { mRoom = rRoom; }
	int32 GetPlrInx(uint32 userId);

	uint32 GetInsId() { return mInsId; }
	uint32 GetGameEntCount() { return (uint32)mLstGameEntity.size(); }
	uint32 GetRoomId() { return mRoomId; }
	uint32 GetPlrCard(uint32 userId, uint32 idx);

	Room* GetRoom() { return mRoom; }
	virtual std::string ToString();
protected:
	virtual bool OnStart() { return true; }
	virtual bool OnClose() { return true; }
	virtual bool OnEnter(GameEntity* aGameEnt) { return true; }
	virtual bool OnLeave(GameEntity* aGameEnt) { return true; }
protected:
	static uint32 sId;
	uint32									mInsId;
	uint32									mRoomId;
	Room*									mRoom;
	std::map<uint8, GameEntity*>			mLstGameEntity;
	Timer									mTimer;

private:
};
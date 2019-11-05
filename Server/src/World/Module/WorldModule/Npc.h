#pragma once

enum NpcType
{
	NT_None,
};

class Npc : public Entity
{
public:
	Npc();
	~Npc();
	virtual bool Destroy();
	uint32 getNpcId() { return mNpcId; }
	void setNpcId(uint32 npcId) { mNpcId = npcId; }
	void setNpcType(uint8 t) { mNpcType = (NpcType)t; }
	uint8 getNpcType() { return (uint8)mNpcType; }
protected:
	virtual bool onEnterMap();
	virtual bool onLeaveMap();

	virtual bool onEnterPlayerView(Player* plr);
	virtual bool onLeavePlayerView(Player* plr);
public:
	uint32 mNpcId;
	NpcType mNpcType;
};


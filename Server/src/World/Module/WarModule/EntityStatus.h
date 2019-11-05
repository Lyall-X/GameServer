#pragma once

enum EntityStatus
{
	ES_IDLE = 1,
	ES_MOVE,
	ES_ATTACT,
	ES_DIE,
};

class EntityIdleStatus : public Status
{
public:
	int32 onEnter();
	int32 onLeave();
	int32 onUpdate(float time, float delay);
};

class EntityMoveStatus : public Status
{
public:
	int32 onEnter();
	int32 onLeave();
	int32 onUpdate(float time, float delay);
protected:
	void getPoint(Entity* ent);
};

class EntityAttackStatus : public Status
{
public:

	int32 onEnter();
	int32 onLeave();
	int32 onUpdate(float32 time, float32 delay);
};
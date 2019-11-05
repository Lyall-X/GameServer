#pragma once

class Object;
class Status
{
public:
	virtual	int32 onEnter() = 0;
	virtual int32 onLeave() = 0;
	virtual int32 onUpdate(float time, float delay) = 0;
	int32 mStatus;
	Object* mTarget;
};

class FSM
{
public:
	FSM(Object* target = 0);
	~FSM();
	Status* Register(int32 status, Status* s);
	void UnRegister(int32 status);
	int32 ChangeStatus(int32 status);
	bool update(float time, float delay);
	int32 getCurrent() { return mCurrentStates ? mCurrentStates->mStatus : 0; }
protected:
	Status* mCurrentStates;
	Object* mTarget;
	std::map<int32, Status*>		mMapStatus;
};
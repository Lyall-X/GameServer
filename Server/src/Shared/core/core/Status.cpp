#include "Shared.hpp"

FSM::FSM(Object* target /*= 0*/):
mCurrentStates(0),
mTarget(target)
{
	
}

FSM::~FSM()
{
	while (mMapStatus.size())
	{
		auto itr = mMapStatus.begin();
		delete itr->second;
		mMapStatus.erase(itr);
	}
}

Status* FSM::Register(int32 status, Status* s)
{
	s->mStatus = status;
	s->mTarget = mTarget;
	auto itr = mMapStatus.find(status);
	if (itr != mMapStatus.end())
		return NULL;

	mMapStatus.insert(std::make_pair(status, s));
	return s;
}

void FSM::UnRegister(int32 status)
{
	auto itr = mMapStatus.find(status);
	if (itr != mMapStatus.end())
	{
		delete itr->second;
		mMapStatus.erase(itr);
	}
}

int32 FSM::ChangeStatus(int32 status)
{
	if (mCurrentStates == NULL)
	{
		auto itr = mMapStatus.find(status);
		if (itr != mMapStatus.end())
		{
			mCurrentStates = itr->second;
			mCurrentStates->onEnter();
		}
	}
	else
	{
		if (mCurrentStates->mStatus != status)
		{
			{
				mCurrentStates->onLeave();
			}

			{
				auto itr = mMapStatus.find(status);
				if (itr != mMapStatus.end())
				{
					mCurrentStates = itr->second;
					mCurrentStates->onEnter();
				}
			}
		}
	}
	
	return 0;
}

bool FSM::update(float time, float delay)
{
	if (mCurrentStates)
		mCurrentStates->onUpdate(time, delay);
	return true;
}


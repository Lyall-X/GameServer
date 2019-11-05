#include "Shared.h"
#include "DateTime.h"
#include "Object.hpp"
#include "EventDispatcher.h"
#include "TimerDispatcher.h"

const char* TimerEvent::TIMER = "onTimer";

int32 Timer::sId = 0;

Timer::Timer()
{

}

Timer::~Timer()
{
	for (auto itr : mMapTimer)
		delete itr.second;

	mMapTimer.clear();
}

int32 Timer::addEventListener(EventCallback callback, Object* thisObject, float32 second)
{
	TimerDispatcher* timerObject = new TimerDispatcher;
	timerObject->addEventListener(TimerEvent::TIMER, callback, thisObject, NULL);
	timerObject->timerId = ++sId;
	timerObject->second = second;
	timerObject->currentTime = (float32)DateTime::GetNowAppUS();
	mMapTimer.insert(std::make_pair(timerObject->timerId, timerObject));
	return timerObject->timerId;
}

int32 Timer::removeEventListener(int32 timerId)
{
	mListRemove.push_back(timerId);
	return 0;
}

void Timer::clear()
{
	for(auto& var : mMapTimer)
	{
		removeEventListener(var.first);
	}
}

void Timer::update(float32 time, float32 delay)
{
	while (mListRemove.size() > 0)
	{
		auto itr = mListRemove.begin();
		auto tItr = mMapTimer.find(*itr);
		if (tItr != mMapTimer.end())
			delete tItr->second;
		mMapTimer.erase(*itr);

		mListRemove.erase(itr);
	}

	for (auto itr : mMapTimer)
	{
		TimerDispatcher& timerDispatcher = *(itr.second);
		int32 t = (int32)(time - timerDispatcher.currentTime);
		if (t >= timerDispatcher.second)
		{
			TimerEvent te;
			te.event = TimerEvent::TIMER;
			te.timerDispatcher = &timerDispatcher;
			timerDispatcher.Dispatch(te);
			timerDispatcher.currentTime = time + (t - timerDispatcher.second);
		}
	}
}


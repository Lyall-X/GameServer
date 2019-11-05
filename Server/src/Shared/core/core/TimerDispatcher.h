#pragma once

class TimerDispatcher;
class TimerEvent : public Event
{
public:
	static const char* TIMER;
public:
	TimerDispatcher* timerDispatcher;
};

class TimerDispatcher : public EventDispatcher
{
public:
	int32 timerId;
	float32 second;
	float32 currentTime;
};


class Timer
{
public:
	Timer();
	~Timer();
	int32 addEventListener(EventCallback callback, Object* thisObject, float32 second);
	int32 removeEventListener(int32 timerId);
	void clear();
	void update(float32 time, float32 delay);
protected:
	static int32 sId;
	std::map<int32, TimerDispatcher*>	mMapTimer;
	std::list<int32>					mListRemove;
};
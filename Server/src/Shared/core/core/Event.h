#pragma once

class EventDispatcher;
class EventRegister;

class Event
{
public:
	Event();
	virtual ~Event() {}

public:
	std::string event;
	EventDispatcher* targetDispatcher;
	EventRegister* targetRegister;
	void* param;
	std::vector<void*> params;
};


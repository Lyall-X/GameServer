#pragma once

class Object;
class Event;

typedef int (Object::*EventCallback) (Event& e);

class EventRegister
{
public:
	static uint32 sId;
public:
	uint32 id = 0;
	std::string event;
	Object* thisObject = NULL;
	EventCallback callback = NULL;
	void* param = NULL;
public:
	EventRegister();
	~EventRegister();

	bool equalListener(const char* name, EventCallback callback, Object* thisObject);
};


#include "Shared.hpp"
uint32 EventRegister::sId = 0;

EventRegister::EventRegister() :
id(++sId)
{
}

EventRegister::~EventRegister()
{
}

bool EventRegister::equalListener(const char* name, EventCallback callback, Object* thisObject)
{
	if (this->event == name && this->callback == callback && this->thisObject == thisObject)
		return true;

	return false;
}

#include "Shared.hpp"

IMPLEMENT_CLASS(EventDispatcher, Object)
EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
	std::map<std::string, std::vector<EventRegister*>>::iterator itr;
	for (itr = mMapListener.begin(); itr != mMapListener.end(); ++itr)
	{
		std::vector<EventRegister*>& listeners = itr->second;
		std::vector<EventRegister*>::iterator l_itr;
		for (l_itr = listeners.begin(); l_itr != listeners.end(); ++l_itr)
		{
			delete (*l_itr);
		}
		listeners.clear();
	}
	mMapListener.clear();
	
}

int32 EventDispatcher::addEventListener(const std::string& name, EventCallback callback, Object* thisObject, void* param /* = NULL */)
{
	EventRegister* lis = new EventRegister();

	lis->event = name;
	lis->thisObject = thisObject;
	lis->callback = callback;
	lis->param = param;

	auto itr = this->mMapListener.find(name);
	std::vector<EventRegister*>* listeners = NULL;
	if (itr != this->mMapListener.end())
	{
		VectorEventRegister& listeners = itr->second;
		listeners.push_back(lis);
	}
	else
	{
		VectorEventRegister ls;
		ls.push_back(lis);
		this->mMapListener.insert(std::make_pair(name, ls));
	}

	return lis->id;
}

int32 EventDispatcher::addEventListener(int32 id, EventCallbackProcess callback, Object* thisObject)
{
	auto itr = mMapCallbackListener.find(id);
	std::vector<std::pair<Object*, EventCallbackProcess>>* listeners = NULL;
	if (itr != this->mMapCallbackListener.end())
	{
		listeners = &(itr->second);
		listeners->push_back(std::make_pair(thisObject, callback));
	}
	else
	{
		std::vector<std::pair<Object*, EventCallbackProcess>> lst;
		lst.push_back(std::make_pair(thisObject, callback));
		mMapCallbackListener.insert(std::make_pair(id, lst));
	}
	return 0;
}

int32 EventDispatcher::removeEventListener(const std::string& name, EventCallback callback, Object* thisObject)
{
	auto itr = this->mMapListener.find(name);
	std::vector<EventRegister*>* listeners = NULL;
	if (itr != this->mMapListener.end())
	{
		VectorEventRegister& listeners = itr->second;

		for (auto l_itr = listeners.begin();
			l_itr != listeners.end();
			++l_itr)
		{
			EventRegister* lis = (*l_itr);
			if (lis->equalListener(name.c_str(), callback, thisObject))
			{
				listeners.erase(l_itr);
				if (listeners.size() <= 0)
					this->mMapListener.erase(itr);
				delete lis;
				return 0;
			}
		}
	}
	return 0;

}

int32 EventDispatcher::removeEventListener(int32 id, EventCallbackProcess callback, Object* thisObject)
{
	auto itr = mMapCallbackListener.find(id);
	if (itr != mMapCallbackListener.end())
	{
		auto& itrListeners = itr->second;
		for (auto itrList = itrListeners.begin(); itrList != itrListeners.end(); ++itrList)
		{
			if (callback == itrList->second && thisObject == itrList->first)
			{
				itrListeners.erase(itrList);
				break;
			}
		}

		if (itrListeners.size() <= 0)
			mMapCallbackListener.erase(itr);
	}

	return 0;
}

int32 EventDispatcher::Dispatch(Event& event)
{
	int32 result = 0;
	auto itr = this->mMapListener.find(event.event);
	std::vector<EventRegister*>* listeners = NULL;
	if (itr != this->mMapListener.end())
	{
		VectorEventRegister& listeners = itr->second;
		for (auto eventRgr : listeners)
		{
			event.targetDispatcher = this;
			event.targetRegister = eventRgr;
			event.param = eventRgr->param;
			try
			{
				if (eventRgr->thisObject == NULL || eventRgr->callback == NULL)
					continue;
				result = (eventRgr->thisObject->*(eventRgr->callback))(event);
			}
			catch (std::string e)
			{
				e = "Registration callback and the definition of the function is not set!";
				throw(e);
			}
		}
	}
	return result;
}

int32 EventDispatcher::Dispatch(int32 id, void* lparam, void* wparam)
{
	int32 result = 0;
	auto itr = mMapCallbackListener.find(id);
	if (itr != this->mMapCallbackListener.end())
	{
		VertorEventCallbackProcess& listeners = itr->second;
		for (auto& callbackPair : listeners)
		{
			if (callbackPair.second == NULL || callbackPair.first == NULL) continue;
			result = (callbackPair.first->*(callbackPair.second))(lparam, wparam);
		}
	}
	return result;
}

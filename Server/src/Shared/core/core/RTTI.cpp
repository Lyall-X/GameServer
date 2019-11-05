#include "Shared.hpp"

typedef Object* (*CreateObjectFunction)();

std::map<std::string, RTTI*>* msRTTI = NULL;

RTTI::RTTI(cstring& className, int objectSize, void* pfnCreateObject, RTTI* baseClass):
mClassName(className),
mObjectSize(objectSize),
mPfnCreateObject((CreateObjectFunction)pfnCreateObject),
mBaseClass(baseClass)
{
	static std::map<std::string, RTTI*> sRTTI;
	if (msRTTI == NULL) msRTTI = &sRTTI;
	msRTTI->insert(std::make_pair(className, this));
}

RTTI::~RTTI(void)
{
	msRTTI->erase(mClassName);
}

Object* RTTI::createObject()
{
	return mPfnCreateObject ? mPfnCreateObject() : NULL;
}

Object* RTTI::createObject(cstring& className)
{
	RTTI* rtti = getRTTI(className);
	if (rtti == NULL) return NULL;
	return rtti->mPfnCreateObject();
}

RTTI* RTTI::getRTTI(cstring& className)
{
	auto itr = msRTTI->find(className);
	if (itr == msRTTI->end()) return NULL;
	return itr->second;
}

Object* RTTI::dynamicCast(Object* object, cstring& className)
{
	RTTI* rtti = object->getThisClass();
	while (rtti) {
		if (rtti->mClassName == className)
			return object;
		rtti = rtti->mBaseClass;
	}
	return NULL;
}

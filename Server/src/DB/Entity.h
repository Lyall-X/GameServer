#pragma once

#include "Shared.h"

class Entity
{
public:
	Entity();
	Guid getGuid() { return mGuid; }
protected:
	static Guid sGuid;
	Guid mGuid;
private:
	
};
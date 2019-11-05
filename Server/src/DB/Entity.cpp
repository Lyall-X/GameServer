#include "stdafx.h"

Guid Entity::sGuid = 0;
Entity::Entity()
{
	mGuid = ++sGuid;
}

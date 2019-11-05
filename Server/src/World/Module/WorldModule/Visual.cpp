#include "stdafx.h"

bool Visual::EnterView(Entity* tar)
{
	mObjects.insert(tar);
	return true;
}

bool Visual::LeaveView(Entity* tar)
{
	mObjects.erase(tar);
	return true;
}

bool Visual::ClearView()
{
	mObjects.clear();
	return true;
}

bool Visual::CheckView(Entity* tar)
{
	auto itr = mObjects.find(tar);
	if (itr != mObjects.end())
		return true;
	return false;
}

#include "stdafx.h"

IMPLEMENT_CLASS(WarModule, Module);

WarModule::WarModule()
{

}

WarModule::~WarModule()
{

}

bool WarModule::Initialize()
{
	return true;
}

bool WarModule::Update(float time, float delay)
{
	return true;
}

bool WarModule::Destroy()
{
	return true;
}

bool WarModule::onEnterWorld(Player* player, Dictionary& dict)
{

	return true;
}

bool WarModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

War* WarModule::CreateWar(Entity* ent)
{
	War* war = new War;
	war->setEntity(ent);
	if (mMapWar.find(ent->getGuid()) != mMapWar.end())
	{
		SAFE_DELETE(war);
		return NULL;
	}
	mMapWar.insert(std::make_pair(ent->getGuid(), war));
	return war;
}

void WarModule::DestroyWar(Entity* ent)
{
	auto itr = mMapWar.find(ent->getGuid());
	if (itr == mMapWar.end())
		return;

	delete itr->second;
	mMapWar.erase(itr);
}

War* WarModule::getWar(Entity* ent)
{
	return getWar(ent->getGuid());
}

War* WarModule::getWar(Guid guid)
{
	auto itr = mMapWar.find(guid);
	if (itr == mMapWar.end())
		return NULL;
	return itr->second;
}

void WarModule::fllowTarget(Entity* ent, Entity* tar)
{
	War* war = getWar(ent);
	if (war == NULL)
		return;

	war->setTarget(tar);
}

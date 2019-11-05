#include "stdafx.h"

IMPLEMENT_CLASS(BackpackModule, Module);

BackpackModule::BackpackModule()
{

}

BackpackModule::~BackpackModule()
{

}

bool BackpackModule::Initialize()
{
	return true;
}

bool BackpackModule::Update(float time, float delay)
{
	return true;
}

bool BackpackModule::Destroy()
{
	return true;
}

bool BackpackModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool BackpackModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

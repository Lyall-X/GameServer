#include "stdafx.h"

IMPLEMENT_CLASS(ItemModule, Module);

ItemModule::ItemModule()
{

}

ItemModule::~ItemModule()
{
}

bool ItemModule::Initialize()
{
	return true;
}

bool ItemModule::Update(float time, float delay)
{
	return true;
}

bool ItemModule::Destroy()
{
	return true;
}

bool ItemModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool ItemModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

#include "stdafx.h"
IMPLEMENT_CLASS(TaskModule, Module);

TaskModule::TaskModule()
{

}

TaskModule::~TaskModule()
{

}

bool TaskModule::Initialize()
{
	return true;
}

bool TaskModule::Update(float time, float delay)
{
	return true;
}

bool TaskModule::Destroy()
{
	return true;
}

bool TaskModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool TaskModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

#pragma once

class TaskModule : public Module
{
	DECLARE_CLASS(TaskModule)
public:
	TaskModule();
	virtual ~TaskModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	std::map<uint32, TaskStepNodeList> mMapTaskStepNodeList;
	std::map<uint32, TaskStepNodeList> mMapFinishTaskStepNodeList;
};
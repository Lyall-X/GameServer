#pragma once

class Module : public EventDispatcher
{
public:
	DECLARE_CLASS(Module)
public:
	Module() {}
	virtual ~Module() {}
	virtual bool Initialize() { return true; }
	virtual bool Update(float time, float delay) { return true; }
	virtual bool Destroy() { return true; }
	virtual bool onEnterWorld(Player* player, Dictionary& dict) { return true; }
	virtual bool onLeaveWorld(Player* player, Dictionary& dict) { return true; }
	virtual bool onLoad(Player* player, Dictionary& bytes) { return true; }
	virtual bool onSave(Player* player, Dictionary& bytes) { return true; }
	virtual bool onLoadEnd(Player* player, Dictionary& bytes) { return true; }
	virtual bool onSaveEnd(Player* player, Dictionary& bytes) { return true; }

};
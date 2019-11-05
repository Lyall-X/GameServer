#pragma once

class BackpackModule : public Module
{
	DECLARE_CLASS(BackpackModule)
public:
	BackpackModule();
	virtual ~BackpackModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
private:
};

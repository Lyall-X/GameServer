#pragma once

class WarModule : public Module
{
	DECLARE_CLASS(WarModule)
public:
	WarModule();
	virtual ~WarModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	War* CreateWar(Entity* ent);
	void DestroyWar(Entity* ent);
	War* getWar(Entity* ent);
	War* getWar(Guid guid);
	void fllowTarget(Entity* ent, Entity* tar);
private:
	std::map<Guid, War*>		mMapWar;
};

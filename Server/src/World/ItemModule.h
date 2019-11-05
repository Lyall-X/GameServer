#pragma once

class Item;
class ItemModule : public Module
{
	DECLARE_CLASS(ItemModule)
public:
	ItemModule();
	virtual ~ItemModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
protected:
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
protected:
	std::map<uint32, Item*>		mMapItem;
};

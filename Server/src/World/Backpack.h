#pragma once


enum GridType
{
	GT_Prop,	 // 道具;
	GT_Goods,	 // 物品;
};

class BackpackGrid
{
public:
	int8		type;
	int8		subType;
	uint32		itemId;
};

class Backpack
{
public:

protected:
	std::vector<BackpackGrid*> mListBackpackGrid;
};
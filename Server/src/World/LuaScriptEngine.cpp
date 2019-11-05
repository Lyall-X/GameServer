#include "stdafx.h"


Object* luaObject()
{
	return LuaEngine::sCurrentObject;
}

Entity* luaEntity()
{
	return (Entity*)LuaEngine::sCurrentObject;
}

Player* luaPlayer()
{
	return (Player*)LuaEngine::sCurrentObject;
}

GameGoldenFlower* luaGameComponent()
{
	return (GameGoldenFlower*)LuaEngine::sCurrentObject;
}

Entity* luaPlrToEnt(Player* player)
{
	return player;
}

Entity* luaNpcToEnt(Npc* npc)
{
	return npc;
}

Player* luaFindPlrByUserId(uint32 userId)
{
	return sWorld.FindPlrByUserId(userId);
}

Player* luaFindPlrByName(cstring& name)
{
	return sWorld.FindPlrByName(name);
}

GameComponent* luaToGameModle(GameGoldenFlower* aGame)
{
	return (GameComponent*)aGame;
}

GameGoldenFlower* luaGoldenFlower(uint32 insId)
{
	return (GameGoldenFlower*)sGame.FindGameModle(insId);
}

GameGoldenFlower* luaPlrGoldenFlower(uint32 userId)
{
	return (GameGoldenFlower*)sGame.FindPlrGameModle(userId);
}

PropertyHelper* luaProperty()
{
	return &sProperty;
}

WorldModule* luaWorld()
{
	return &sWorld;
}

RoomModule* luaRoom()
{
	return &sRoom;
}

GameModule* luaGame()
{
	return &sGame;
}

FriendsModule* luaFriends()
{
	return &sFriends;
}

ConfigManager* luaCfg()
{
	return &sCfgMgr;
}

ShopModule* luaShop()
{
	return &sShop;
}

MailModule* luaMail()
{
	return &sMails;
}

//luabind::object GetTestTable(lua_State* luaState)
//{
//	luabind::object obj = luabind::newtable(luaState);
//	for (uint32 i = 1; i <= 10; ++i)
//	{
//		obj[i] = i;
//	}
//	int k = luabind::tablecount(obj);
//	return obj;
//}

//luabind::object GetTestTable1(luabind::object& obj)
//{
//	return obj;
//}

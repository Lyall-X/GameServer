#pragma once

//
class Object;
class Entity;
class Player;
class Npc;

class PropertyHelper;
class GameComponent;
class GameGoldenFlower;

class WorldModule;
class RoomModule;
class GameModule;
class FriendsModule;
class ShopModule;
class MailModule;

Object* luaObject();
Entity* luaEntity();
Player* luaPlayer();
GameGoldenFlower* luaGameComponent();

Entity* luaPlrToEnt(Player* player);
Entity* luaNpcToEnt(Npc* npc);
Player* luaFindPlrByUserId(uint32 userId);
Player* luaFindPlrByName(cstring& name);

GameComponent* luaToGameModle(GameGoldenFlower* aGame);
GameGoldenFlower* luaGoldenFlower(uint32 insId);
GameGoldenFlower* luaPlrGoldenFlower(uint32 userId);

PropertyHelper* luaProperty();
WorldModule* luaWorld();
RoomModule* luaRoom();
GameModule* luaGame();
FriendsModule* luaFriends();
ConfigManager* luaCfg();
ShopModule* luaShop();
MailModule* luaMail();

//luabind::object GetTestTable(lua_State* luaState);
//luabind::object GetTestTable1(luabind::object& obj);
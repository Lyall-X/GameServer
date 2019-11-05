#pragma once
#include "Shared.hpp"
//////////////////////////////////////////////////////////////////////////
#include <json/json.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>
//////////////////////////////////////////////////////////////////////////

#include "protocols/protocols.h"
#include "Channel.h"
#include "CfgDef.h"
#include "ConfigManager.h"
#include "Property.h"

#include "Session.h"
#include "SessionManager.h"


#ifndef LINUX


#ifdef WIN64

#ifdef _DEBUG


#pragma comment(lib, "hiredis64_d.lib")
#pragma comment(lib, "Shared64_d.lib")
#pragma comment(lib, "lua64_d.lib")
#pragma comment(lib, "luabind64_d.lib")


#else

#pragma comment(lib, "hiredis64.lib")
#pragma comment(lib, "Shared64.lib")
#pragma comment(lib, "lua64.lib")
#pragma comment(lib, "luabind64.lib")

#endif // DEBUG

#else

#ifdef _DEBUG

#pragma comment(lib, "hiredis_d.lib")
#pragma comment(lib, "Shared_d.lib")
#pragma comment(lib, "lua_d.lib")
#pragma comment(lib, "luabind_d.lib")
#pragma comment(lib, "python/python36_d.lib")
#else

#pragma comment(lib, "hiredis.lib")
#pragma comment(lib, "Shared.lib")
#pragma comment(lib, "lua.lib")
#pragma comment(lib, "luabind.lib")
#pragma comment(lib, "python/python36.lib")
#endif // DEBUG

#endif

#endif // !LINUX



#include "MsgCode.h"
#include "ConstDef.h"
#include "RedisDef.h"
#include "LuaEngine.h"
#include "LuaScriptEngine.h"

#include "Item.h"
#include "Backpack.h"

#include "Module.h"
#include "ItemModule.h"
#include "BackpackModule.h"

#include "BackpackModule.h"
#include "ItemModule.h"


#include "./Module/WorldModule/Visual.h"
#include "./Module/WorldModule/Entity.h"
#include "./Module/WorldModule/Player.h"
#include "./Module/WorldModule/Npc.h"
#include "./Module/WorldModule/PlayerHandler.h"
#include "./Module/WorldModule/WorldModule.h"

#include "./Module/MapModule/Map.h"
#include "./Module/MapModule/MapModule.h"
#include "./Module/WarModule/Skill.h"
#include "./Module/WarModule/EntityStatus.h"

#include "./Module/WarModule/WarHandler.h"
#include "./Module/WarModule/War.h"
#include "./Module/WarModule/WarModule.h"
#include "./Module/FriendsModule/FriendsHandler.h"
#include "./Module/FriendsModule/Friends.h"
#include "./Module/FriendsModule/FriendsModule.h"

#include "./Module/TeamModule/TeamHandler.h"
#include "./Module/TeamModule/Team.h"
#include "./Module/TeamModule/TeamModule.h"
#include "./Module/RoomModule/RoomMatch.h"
#include "./Module/RoomModule/RoomHandler.h"
#include "./Module/RoomModule/Room.h"
#include "./Module/RoomModule/RoomModule.h"

#include "./Module/GameModule/GameHandler.h"
#include "./Module/GameModule/GameComponent.h"
#include "./Module/GameModule/GameGoldenFlower.h"
#include "./Module/GameModule/GameModule.h"

#include "./Module/MailModule/Mail.h"
#include "./Module/Mailmodule/MailModule.h"

#include "./Module/ShopModule/ShopHandler.h"
#include "./Module/ShopModule/Shop.h"
#include "./Module/ShopModule/ShopModule.h"

#include "./Module/RankModule/RankModule.h"

#include "./Module/TaskModule/TaskNode.h"
#include "./Module/TaskModule/TaskModule.h"

#include "Utils.h"
#include "DBHandler.h"
#include "SessionHandler.h"
#include "RedisProxy.h"

#include "Application.h"


#define sApp INSTANCE(Application)
#define sPacketMgr INSTANCE(PacketManager)
#define sCfgMgr INSTANCE(ConfigManager)
#define sSsnMgr INSTANCE(SessionManager)
#define sRedisProxy INSTANCE(RedisProxy)
#define sLua INSTANCE(LuaEngine)
#define sProperty INSTANCE(PropertyHelper)
#define sObjectMgr INSTANCE(ObjectManager)

#define sWorld (*GetModule(WorldModule))
#define sMap (*GetModule(MapModule))
#define sTeam (*GetModule(TeamModule))
#define sFriends (*GetModule(FriendsModule))
#define sRoom (*GetModule(RoomModule))
#define sGame (*GetModule(GameModule))
#define sShop (*GetModule(ShopModule))
#define sRank (*GetModule(RankModule))
#define sMails (*GetModule(MailModule))


#define RegDBEvent(id, cb, thisObject) RegisterEventProcess(GetDBServer(), id, cb, thisObject)
#define RegWorldEvent(id, cb, thisObject) RegisterEventProcess((GetWorldServer()), id, cb, thisObject)
#pragma once

#include "Shared.hpp"


#include "ConfigManager.h"
#include "Entity.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Session.h"
#include "SessionManager.h"
#include "protocols/protocols.h"
#include "ConstDef.h"
#include "MsgCode.h"
#include "RedisDef.h"

#include "mysql.hpp"
#include "DBTable.h"
#include "SessionHandler.h"
#include "RedisProxy.h"

#include "Application.h"

#ifndef LINUX


#ifdef WIN64

	#ifdef _DEBUG

		#pragma comment(lib, "libmysql/libmysql64.lib")
		#pragma comment(lib, "hiredis64_d.lib")
		#pragma comment(lib, "mysql64_d.lib")
		#pragma comment(lib, "Shared64_d.lib")

	#else
		#pragma comment(lib, "libmysql/libmysql64.lib")
		#pragma comment(lib, "hiredis64.lib")
		#pragma comment(lib, "mysql64.lib")
		#pragma comment(lib, "Shared64.lib")

	#endif // DEBUG

#else
	#pragma comment(lib, "libmysql/libmysql.lib")
	#ifdef _DEBUG
	#pragma comment(lib, "hiredis_d.lib")
	#pragma comment(lib, "mysql_d.lib")
	#pragma comment(lib, "Shared_d.lib")

	#else
	#pragma comment(lib, "hiredis.lib")
	#pragma comment(lib, "mysql.lib")
	#pragma comment(lib, "Shared.lib")

	#endif // DEBUG

#endif

#endif // !LINUX

#define sRedisProxy INSTANCE(RedisProxy)
#define sApp INSTANCE(Application)
#define sSsnMgr INSTANCE(SessionManager)
#define sPlrMgr INSTANCE(PlayerManager)
#define sPacketMgr INSTANCE(PacketManager)
#define sDBAngent INSTANCE(DBAngent)
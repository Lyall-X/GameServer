#pragma once

#include "Shared.hpp"
#include "protocols/protocols.h"
#include "MsgCode.h"
#include "ConstDef.h"
#include "Channel.h"
#include "Property.h"
#include <windows.h>
#include "ConfigManager.h"
#include "Robot.h"
#include "RobotManager.h"
#include "SocketHandler.h"
#include "Utils.h"


//#pragma comment(lib, "libevent/libevent.lib")
//#pragma comment(lib, "libevent/libevent_core.lib")
//#pragma comment(lib, "libevent/libevent_extras.lib")



#ifndef LINUX

#pragma comment(lib, "ws2_32.lib")

#ifdef WIN64

#ifdef _DEBUG

#pragma comment(lib, "Shared64_d.lib")

#else

#pragma comment(lib, "Shared64.lib")

#endif // DEBUG

#else

#ifdef _DEBUG

#pragma comment(lib, "Shared_d.lib")

#else

#pragma comment(lib, "Shared.lib")

#endif // DEBUG

#endif

#endif // !LINUX

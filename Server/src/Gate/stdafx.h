#pragma once
#include "Shared.hpp"
#include "ConfigManager.h"
#include "Session.h"
#include "Entity.h"
#include "Player.h"
#include "SessionManager.h"
#include "protocols/protocols.h"
#include "ConstDef.h"
#include "Application.h"

#ifndef LINUX


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
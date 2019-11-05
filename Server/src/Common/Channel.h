#pragma once

enum EnumChannel : int8
{
	EC_NONE,
	EC_WORLD = 1,				// 世界频道
	EC_SYSTEM,					// 系统消息
	EC_MAP,						// 当前地图频道
	EC_VIEW,					// 当前视野频道
	EC_TEAM,					// 当前队伍频道
	EC_ROOM,					// 房间频道
	EC_TARGET,					
};
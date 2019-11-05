#pragma once

static const char* sScriptPlayer = "player";
static const char* sScriptRoom = "room";
static const char* sScriptGame = "game";
static const char* sScriptTeam = "team";
static const char* sScriptCard = "card";

static const char* sKey = "ogn";

static const uint32 sSpeakTotalTime = 30;
static const uint32 sMaxRoleCount = 3;


enum SndTarget : char
{
	Snd_Ssn,
	Snd_Plr,
	Snd_Sck,
};

enum CardType
{
	CT_None = 0,
	CT_Block,			// 方
	CT_Club,			// 梅
	CT_Heart,			// 红
	CT_Spade,			// 黑
	CT_Joker,			// 鬼
};

enum JokerType
{
	JT_None,
	JT_Small,			// 小
	JT_Big,				// 大
};

enum ThreeCardType
{
	TCT_None,
	TCT_Danzhang,				// 单
	TCT_Duizi,					// 对
	TCT_Shunzi,					// 顺
	TCT_Tonghua,				// 同
	TCT_Tonghuashun,			// 同顺
	TCT_Baozi,					// 豹
};

enum GameOperateType
{
	GOperT_None = 0,
	GOperT_See,						// 看
	GOperT_Chipin,					// 下
	GOperT_CallChipin,				// 跟
	GOperT_VS,						// 比
};

enum VSType
{
	VST_None,
	VST_Win,
	VST_Lose,
};
#pragma once

enum MsgCode
{
	MC_None = 0,
	MC_LoginOk,
	MC_LoginTheWrongAccountOrPassword,
	MC_LoginTheNumberInTheRemote,
	MC_LoginYouHaveLanded,
	MC_LoginSelectRole,			// 已经选好角色
	MC_InviterHaveTeam,
	MC_JoinerTeamNotExist,
	MC_HaveTeam,
	MC_TeamNotExist,
	MC_AgreeAddTeamNotLeader, // 同意申请加入队伍的不是队长;
	MC_TeamError,

	MC_RoomFull,					// 
};

enum NetResult
{
	NResultFail,				// 失败
	NResultSuccess,				// 成功
};
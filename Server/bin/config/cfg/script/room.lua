dofile("../config/cfg/script/global.lua");

function OnCreate(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:FindPlrByUserId(userId);
	if play then
		play:sendPacketToMsg(EC_TARGET, "["..play:getName().."]创建["..roomId.."]房间");
	end
	return 0;
end
function OnClose(roomId)

	return 0;
end

function OnEnter(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:FindPlrByUserId(userId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]加入["..roomId.."]房间");
		
	end
	return 0;
end

function OnLeave(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:FindPlrByUserId(userId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]离开["..roomId.."]房间");
	end
	return 0;
end

function OnChangeMaster(roomId, oldUserId, newUserId)
	local sWorld = luaWorld();
	local play = sWorld:FindPlrByUserId(newUserId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..roomId.."]房间更换房主为["..newUserId.."]");
	end
	return 0;
end

function OnChangeState(roomId, userId, oldState, state)
	local sWorld = luaWorld();
	local play = sWorld:FindPlrByUserId(userId);
	if play then
		local str = "";
		if state == RPS_None then
			str = "取消准备";
		end
		
		if state == RPS_Ready then
			str = "准备";	
		end
		if state == RPS_Game then
			str = "开始游戏";	
		end
		
		if state == RPS_Observed then
			str = "观点中";	
		end
		play:sendPacketToMsg(EC_ROOM, "房间ID["..roomId.."]玩家ID["..userId.."]"..str);
	end
	return 0;
end
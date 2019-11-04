
dofile("../config/cfg/script/global.lua");

function OnStart(gameId)
	local sGame = luaGame();
	local agame = sGame:FindGameModle(gameId);
	local roomId = agame:GetRoomId();
	if agame then
		local num = agame:GetGameEntCount();
		for i=1,num do
			local gameEnt = agame:GetGameEnt(i - 1);
			if gameEnt then
				local play = luaFindPlrByUserId(gameEnt.userId);
				if play then
					play:sendPacketToMsg(EC_SYSTEM, "["..roomId.."]房间游戏开始！");
				end
			end
					
		end

	end
	return 0;
end

function OnClose(gameId)
	local sGame = luaGame();
	local agame = sGame:FindGameModle(gameId);
	local roomId = agame:GetRoomId();
	if agame then
		local num = agame:GetGameEntCount();
		for i=1,num do
			local gameEnt = agame:GetGameEnt(i - 1);
			if gameEnt then
				local play = luaFindPlrByUserId(gameEnt.userId);
				if play then
					play:sendPacketToMsg(EC_SYSTEM, "["..roomId.."]房间游戏结束！");
				end
			end
					
		end

	end
	return 0;
end

function OnEnter(gameId, userId)
	local aGame = luaGoldenFlower(gameId);
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	local count = aGameEnt:GetCardCount();
	local str = "";
	local sCfgMgr = luaCfg();
		
	for i = 0, count - 1, 1 do
		local card = aGameEnt:GetCard(i);
		local cardinfo = sCfgMgr:getCardJson(card);
		str = str.."["..cardinfo.Name.."]";
	end	
	play:sendPacketToMsg(EC_SYSTEM, str);
	return 0;
end

function OnLeave(gameId, userId)

	return 0;
end

function OnSeeCard(userId)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]".."看牌");
	return 0;
end

-- 下
function OnChipinReq(userId, gold)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]".."下注"..gold.."金币");
	return 0;
end

--跟
function OnCallReq(userId, gold)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]".."跟注"..gold.."金币");
	return 0;
end

function OnCompareReq(userId, tarUserId, result)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	local tarPlay = luaFindPlrByUserId(tarUserId);
	local strret = result == 1 and "赢" or "输";
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]比牌".."["..tarPlay:getName().."]["..strret.."]");
	return 0;
end
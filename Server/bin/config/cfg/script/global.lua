maxplayer = 1000;
updatetime = 1.55;
serverIp = "192.168.0.1";

-- 频道 --
EC_WORLD = 1;
EC_SYSTEM = 2;
EC_MAP = 3;
EC_VIEW = 4;
EC_TEAM = 5;
EC_ROOM = 6;
EC_TARGET = 7;

-- 房间状态 --
RPS_None = 0;				-- 正常
RPS_Ready = 1;				-- 准备
RPS_Game = 2;				-- 游戏中
RPS_Observed = 3;			-- 观战中


TCT_Danzhang = 1;				-- 单
TCT_Duizi = 2;					-- 对
TCT_Shunzi = 3;					-- 顺
TCT_Tonghua = 4;				-- 同
TCT_Tonghuashun = 5;			-- 同顺
TCT_Baozi = 6;					-- 豹

function string.split(str, delimiter)
	if str==nil or str=='' or delimiter==nil then
		return nil
	end
	
    local result = {}
    for match in (str..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match)
    end
    return result;
end
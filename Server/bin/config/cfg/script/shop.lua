dofile("../config/cfg/script/global.lua");

function OnSellProduct(userId, productInsId)
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg("³öÊÛ:"..productInsId);
	return 0;
end
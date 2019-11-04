
local gmManager = {};

local function RegGm(obj)
	if (obj.name ~= nil) then
		gmManager[obj.name] = obj;
	end
end

function GmCmd(s1, s2, s3, s4)
	local player = luaPlayer();
	gmManager[s1].fun(player, s2, s3, s4);
	return 0;
end

RegGm(
{
name = "test";
fun=function (player, s1, s2, s3)
	local entity = luaEntity();
	local property = luaProperty();
	if property == nil then
	end
	property:setMaxHp(entity, 1);
end
});

RegGm(
{
name = "addgold";
fun=function (player, s1, s2, s3)
	local property = luaProperty();
	property:setGold(player, tonumber(s1));
end
});

RegGm(
{
name = "croom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoCreateRoom(player);
end
});

RegGm(
{
name = "enroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoEnterRoom(player, tonumber(s1));
end
});

RegGm(
{
name = "leroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoLeaveRoom(player, tonumber(s1));
end
});


RegGm(
{
name = "cmroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoChangeRoomMaster(player, tonumber(s1), tonumber(s2));
end
});


RegGm(
{
name = "lstroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoRoomList(player, tonumber(s1), tonumber(s2));
end
});

RegGm(
{
name = "reroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoRoomReady(player, tonumber(s1));
end
});

RegGm(
{
name = "sgroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoRoomStartGame(player);
end
});

RegGm(
{
name = "clroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoRoomClose(player);
end
});

RegGm(
{
name = "auroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoAutoMatch(player);
end
});

RegGm(
{
name = "kpai";
fun=function (play, s1, s2, s3)
	local sGame = luaGame();
	sGame:DoOperateSee(play);
end
});

RegGm(
{
name = "xzhu";
fun=function (play, s1, s2, s3)
	local sGame = luaGame();
	sGame:DoOperateChipinReq(play, tonumber(s1));
end
});

RegGm(
{
name = "gzhu";
fun=function (play, s1, s2, s3)
	local sGame = luaGame();
	sGame:DoOperateCallReq(play);
end
});

RegGm(
{
name = "bpai";
fun=function (play, s1, s2, s3)
	local sGame = luaGame();
	local tarPlr = luaFindPlrByName(s1);
	sGame:DoOperateCompareReq(play, tarPlr:getUserId());
end
});

RegGm(
{
name = "addfrd";
fun=function (play, s1, s2, s3)
	local sFriends = luaFriends();
	local tarPlr = luaFindPlrByName(s1);
	if (tarPlr ~= nil) then
		sFriends:MutualBindFriend(play, tarPlr);
	end
end
});

RegGm(
{
name = "getfrds";
fun=function (play, s1, s2, s3)
	local sFriends = luaFriends();
	sFriends:DoFriendsList(play);
end
});

RegGm(
{
name = "sell";
fun=function (play, s1, s2, s3)
	local sShop = luaShop();
	sShop:DoAddProduct(play, tonumber(s1));
end
});

RegGm(
{
name = "productlst";
fun=function (play, s1, s2, s3)
	local sShop = luaShop();
	sShop:DoFindProductList(play, tonumber(s1), tonumber(s2));
end
});

RegGm(
{
name = "sendmail1";
fun=function (play, s1, s2, s3)
	local sMails = luaMail();
	sMails:SendMail(s1, s2);
end
});

RegGm(
{
name = "sendmail2";
fun=function (play, s1, s2, s3)
	local sMails = luaMail();
	sMails:SendMail(play, MailType.MailT_Target, s1, s2, s3, "");
end
});

RegGm(
{
name = "changename";
fun=function (play, s1, s2, s3)
	local sWorld = luaWorld();
	sWorld:ChangeName(play, s1);
end
});

RegGm(
{
name = "sendmsg";
fun=function (play, s1, s2, s3)
	local sWorld = luaWorld();
	sWorld:sendPacketToMsg(tonumber(s1), s2, play);
end
});
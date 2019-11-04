dofile("../config/cfg/script/global.lua");

CT_Block = 1;
CT_Club = 2;
CT_Heart = 3;
CT_Spade = 4;
CT_Joker = 5;

JT_Small = 1;
JT_Big = 2;


function SortCards(cards)
	if cards[2].Number > cards[3].Number then
		local swap = cards[3];
		cards[3] = cards[2];
		cards[2] = swap;
	end

	if cards[1].Number > cards[3].Number then
		local swap = cards[3];
		cards[3] = cards[1];
		cards[1] = swap;
	end	
	
	if cards[1].Number > cards[2].Number then
		local swap = cards[1];
		cards[1] = cards[2];
		cards[2] = swap;
	end
	return cards;
end

function IsBaozi(card1, card2, card3)
	print(card1.Number.." "..card2.Number.." "..card3.Number);
	if card1.Number == card2.Number and card1.Number == card3.Number then
		return 1;
	end
	return 0;
end

function IsTonghuashun(card1, card2, card3)
	if IsTonghua(card1, card2, card3) == 0 then
		return 0;
	end
	
	return IsShunzi(card1, card2, card3);
end

function IsTonghua(card1, card2, card3)
	if card1.Color == card2.Color and card1.Color == card3.Color then
		return 1;
	end
	return 0;
end

function IsShunzi(card1, card2, card3)
	if (card1.Number + 1 == card2.Number and
		card2.Number + 1 == card3.Number)then
		return 1;
	end

	if (card3.Number - card1.Number == 12 and
		card2.Number + 1 == card3.Number)then
		return 1;
	end
	return 0;
end

function IsDuizi(card1, card2, card3)
	if (card1.Number == card2.Number or 
		card1.Number == card3.Number or 
		card2.Number == card3.Number) then
		return 1;
	end
	return 0;
end

function IsDanzhang(card1, card2, card3)
	return 1;
end

function CheckThreeCardType(card1, card2, card3)

	if (IsBaozi(card1, card2, card3) == 1) then
		return TCT_Baozi;
	end
	
	if (IsTonghuashun(card1, card2, card3) == 1) then
		return TCT_Tonghuashun;
	end
	
	if (IsTonghua(card1, card2, card3) == 1) then
		return TCT_Tonghua;
	end
	
	if (IsShunzi(card1, card2, card3) == 1) then
		return TCT_Shunzi;
	end
	
	if (IsDuizi(card1, card2, card3) == 1) then
		return TCT_Duizi;
	end
	
	if (IsDanzhang(card1, card2, card3) == 1) then
		return TCT_Danzhang;
	end
	return 0;
end

function CheckDanzhang(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];
	
	if u1card1.Number == 1 and u2card1.Number ~= 1 then
		return 1;
	end
	
	if u1card1.Number ~= 1 and u2card1.Number == 1 then
		return 2;
	end

	if u1card3.Number > u2card3.Number then
		return 1;
	end

	if u1card3.Number < u2card3.Number then
		return 2;
	end
	
	if u1card2.Number > u2card2.Number then
		return 1;
	end
	
	if u1card2.Number < u2card2.Number then
		return 2;
	end
	
	if u1card1.Number > u2card1.Number then
		return 1;
	end

	if u1card1.Number < u2card1.Number then
		return 2;
	end

	return 0;
end

function CheckDuizi(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];

	local u1duizi = 0;
	local u2duizi = 0;
	local u1danz = 0;
	local u2danz = 0;
	
	if u1card1.Number == u1card2.Number then
		u1duizi = u1card1.Number;
		u1danz = u1card3.Number;
	else
		if u1card1.Number == u1card3.Number then
			u1duizi = u1card1.Number;
			u1danz = u1card2.Number;
		else
			u1duizi = u1card3.Number;
			u1danz = u1card1.Number;
		end
	end
	
	if u2card1.Number == u2card2.Number then
		u2duizi = u2card1.Number;
		u2danz = u2card3.Number;
	else
		if u2card1.Number == u2card3.Number then
			u2duizi = u2card1.Number;
			u2danz = u2card2.Number;
		else
			u2duizi = u2card3.Number
			u2danz = u2card1.Number;
		end
	end
	
	if u1duizi > u2duizi then
		return 1;
	end
	if u1duizi < u2duizi then
		return 2;
	end
	
	if u1danz > u2danz then
		return 1;
	end
	if u1danz < u2danz then
		return 2;
	end
			
	return 0;
end

function CheckShunzi(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];

	if u1card3.Number > u2card3.Number then
		return 1;
	end
	
	if u1card3.Number < u2card3.Number then
		return 2;
	end

	if u1card2.Number > u2card2.Number then
		return 1;
	end
		
	if u1card2.Number < u2card2.Number then
		return 2;
	end	
	
	if u1card1.Number > u2card1.Number then
		return 1;
	end
		
	if u1card1.Number < u2card1.Number then
		return 2;
	end	
	
	return 0;
end

function CheckTonghua(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];	

	local ret = CheckDanzhang(cards1, cards2);

	if ret == 1 or ret == 2 then
		return ret;
	end
	
	if u1card3.Color > u2card3.Color then
		return 1;
	end

	if u1card3.Color < u2card3.Color then
		return 2;
	end
			
	return 0;
end

function CheckTonghuashun(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];	
	
	local ret = CheckShunzi(cards1, cards2);

	if ret == 1 or ret == 2 then
		return ret;
	end

	if u1card3.Color > u2card3.Color then
		return 1;
	end

	if u1card3.Color < u2card3.Color then
		return 2;
	end
		
	return 0;
end

function CheckBaozi(cards1, cards2)
	local u1card1 = cards1[1];
	local u2card3 = cards2[3];	
	if (u1card1.Number > u2card1.Nu)then
		return 1;
	end
	return 2;
end

function CompareCardsType(type, cards1, cards2)
	if  type == TCT_Danzhang then
		return CheckDanzhang(cards1, cards2);
	end

	if  type == TCT_Duizi then
		return CheckDuizi(cards1, cards2);
	end

	if  type == TCT_Shunzi then
		return CheckShunzi(cards1, cards2);
	end

	if  type == TCT_Tonghua then
		return CheckTonghua(cards1, cards2);
	end

	if  type == TCT_Tonghuashun then
		return CheckTonghuashun(cards1, cards2);
	end

	if  type == TCT_Baozi then
		return CheckBaozi(cards1, cards2);
	end
end

function CompareNumberCard(cards1, cards2)
	local u1cards = split(cards1, ",");
	local u2cards = split(cards2, ",");
	
	local sCfgMgr = luaCfg();
	local u1card1 = sCfgMgr:GetCardJson(tonumber(u1cards[1]));
	local u1card2 = sCfgMgr:GetCardJson(tonumber(u1cards[2]));
	local u1card3 = sCfgMgr:GetCardJson(tonumber(u1cards[3]));
	
	local u2card1 = sCfgMgr:GetCardJson(tonumber(u2cards[1]));
	local u2card2 = sCfgMgr:GetCardJson(tonumber(u2cards[2]));
	local u2card3 = sCfgMgr:GetCardJson(tonumber(u2cards[3]));
	
	local cards1 = {u1card1, u1card2, u1card3};
	local cards2 = {u2card1, u2card2, u2card3};
	
	return CompareCards(cards1, cards2);	
end

function testfun(cards1, cards2)
	local ret = CompareStringCard("梅花A,方块2,黑桃3", "黑桃K,红桃Q,方块A");
	print (ret);
	return 1;
end

function CompareStringCard(cards1, cards2)
	local u1cards = string.split(cards1, ",");
	local u2cards = string.split(cards2, ",");
		
	local sCfgMgr = luaCfg();
	local u1card1 = sCfgMgr:getCardJsonByName(u1cards[1]);
	local u1card2 = sCfgMgr:getCardJsonByName(u1cards[2]);
	local u1card3 = sCfgMgr:getCardJsonByName(u1cards[3]);
	
	local u2card1 = sCfgMgr:getCardJsonByName(u2cards[1]);
	local u2card2 = sCfgMgr:getCardJsonByName(u2cards[2]);
	local u2card3 = sCfgMgr:getCardJsonByName(u2cards[3]);
	
	local cards1 = {u1card1, u1card2, u1card3};
	local cards2 = {u2card1, u2card2, u2card3};
	
	return CompareCards(cards1, cards2);	
end

function ComparePlrCard(userId1, userId2)
	local aGame = luaGame();
	local user1card1 = aGame:GetPlrCard(userId1, 0);
	local user1card2 = aGame:GetPlrCard(userId1, 1);
	local user1card3 = aGame:GetPlrCard(userId1, 2);
	
	local user2card1 = aGame:GetPlrCard(userId2, 0);
	local user2card2 = aGame:GetPlrCard(userId2, 1);
	local user2card3 = aGame:GetPlrCard(userId2, 2);

	local sCfgMgr = luaCfg();
	local u1card1 = sCfgMgr:GetCardJson(user1card1);
	local u1card2 = sCfgMgr:GetCardJson(user1card2);
	local u1card3 = sCfgMgr:GetCardJson(user1card3);
	
	local u2card1 = sCfgMgr:GetCardJson(user2card1);
	local u2card2 = sCfgMgr:GetCardJson(user2card2);
	local u2card3 = sCfgMgr:GetCardJson(user2card3);
	
	local cards1 = {u1card1, u1card2, u1card3};
	local cards2 = {u2card1, u2card2, u2card3};
	return CompareCards(cards1, cards2);	
end

-- 1.大于2.小于3.等于--
function CompareCards(cards1, cards2)
	cards1 = SortCards(cards1);
	cards2 = SortCards(cards2);		
	if CheckRepeat(cards1, cards2) then
		return 3;
	end
	
	local u1cardtype = CheckThreeCardType(cards1[1], cards1[2], cards1[3]);
	local u2cardtype = CheckThreeCardType(cards2[1], cards2[2], cards2[3]);

	if (u1cardtype > u2cardtype) then
		return 1;
	end
	
	if (u1cardtype < u2cardtype) then
		return 2;
	end
	
	local ret = CompareCardsType(u1cardtype, cards1, cards2);
	if (ret == 0) then
		return 1;
	end
	return ret;
end

function CheckRepeat(cards1, cards2)
	if cards1[1].ID == cards1[2].ID then
		return true;
	end
	
	if cards1[1].ID == cards1[3].ID then
		return true;
	end
			
	if cards1[1].ID == cards2[1].ID then
		return true;
	end
	
	if cards1[1].ID == cards2[2].ID then
		return true;
	end
	
	if cards1[1].ID == cards2[3].ID then
		return true;
	end
	
	if cards1[2].ID == cards1[3].ID then
		return true;
	end
			
	if cards1[2].ID == cards2[1].ID then
		return true;
	end

	if cards1[2].ID == cards2[2].ID then
		return true;
	end

	if cards1[2].ID == cards2[3].ID then
		return true;
	end
	
	if cards1[3].ID == cards2[1].ID then
		return true;
	end

	if cards1[3].ID == cards2[2].ID then
		return true;
	end

	if cards1[3].ID == cards2[3].ID then
		return true;
	end
	
	if cards2[1].ID == cards2[2].ID then
		return true;
	end

	if cards2[1].ID == cards2[3].ID then
		return true;
	end
	
	if cards2[2].ID == cards2[3].ID then
		return true;
	end
			
	return false;
end
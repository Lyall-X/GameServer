#pragma once

enum PACKET_ID_ENUM {
ID_NetFirst,
ID_NetSessionEnterNotify,
ID_NetSessionLeaveNotify,
ID_NetPingNotify,
ID_NetPingGateNotify,
ID_NetLoginReq,
ID_NetLoginRes,
ID_NetPlayerSaveNotify,
ID_NetCreateRoleReq,
ID_NetCreateRoleRes,
ID_NetSelectRoleReq,
ID_NetSelectRoleRes,
ID_NetChangeNameReq,
ID_NetChangeNameRes,
ID_NetGmMsg,
ID_NetQueryRoleReq,
ID_NetQueryRoleRes,
ID_NetPropertyReq,
ID_NetPropertyRes,
ID_NetEntityPropertyNotify,
ID_NetEntityEnterMapNotify,
ID_NetEntityLeaveMapNotify,
ID_NetPlayerEnterViewNotify,
ID_NetPlayerLeaveViewNotify,
ID_NetNpcEnterViewNotify,
ID_NetNpcLeaveViewNotify,
ID_NetEntityMoveNotify,
ID_NetEntityMoveToNotify,
ID_NetEntityAttackNotify,
ID_NetEntityFollowNotify,
ID_NetEntityCancelFollowNotify,
ID_NetChatMsgNotify,
ID_NetResponseMsgNotify,
ID_NetCreateTeamReq,
ID_NetCreateTeamRes,
ID_NetOrganizeTeamReq,
ID_NetOrganizeTeamRes,
ID_NetAgreeTeamReq,
ID_NetTeamInfoRes,
ID_NetTeamListReq,
ID_NetTeamListRes,
ID_NetCreateRoomReq,
ID_NetCreateRoomRes,
ID_NetEnterRoomReq,
ID_NetEnterRoomRes,
ID_NetEnterRoomNotify,
ID_NetLeaveRoomReq,
ID_NetLeaveRoomRes,
ID_NetChangeRoomMasterReq,
ID_NetChangeRoomMasterRes,
ID_NetRoomListReq,
ID_NetRoomListRes,
ID_NetRoomReadyReq,
ID_NetRoomReadyRes,
ID_NetRoomStartGameReq,
ID_NetRoomStartGameRes,
ID_NetRoomInfoNotify,
ID_NetGameStartNotify,
ID_NetGameCloseNotify,
ID_NetGameInfoNotify,
ID_NetGameOperateSeeReq,
ID_NetGameOperateSeeRes,
ID_NetGameOperateGiveupReq,
ID_NetGameOperateGiveupRes,
ID_NetGameOperateChipinReq,
ID_NetGameOperateChipinRes,
ID_NetGameOperateCallReq,
ID_NetGameOperateCallRes,
ID_NetGameOperateCompareReq,
ID_NetGameOperateCompareRes,
ID_NetAddFriendReq,
ID_NetAddFriendRes,
ID_NetAddFriendNotify,
ID_NetFriendListReq,
ID_NetFriendListRes,
ID_NetSellProductReq,
ID_NetSellProductRes,
ID_NetProductListReq,
ID_NetProductListRes,
ID_NetMailListNotify,
ID_NetEnd,

};

class DBAccountInfo : public Object {
public:
	DBAccountInfo() {
id = 0;
user = "";

name = "";
email = "";
phone = "";

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << id);
CHECK(bytes << user);
CHECK(bytes << password);
CHECK(bytes << name);
CHECK(bytes << email);
CHECK(bytes << phone);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> id);
CHECK(bytes >> user);
CHECK(bytes >> password);
CHECK(bytes >> name);
CHECK(bytes >> email);
CHECK(bytes >> phone);

		return true;
	}
public:
uint32 id;
std::string user;
BinaryStream password;
std::string name;
std::string email;
std::string phone;

};

class DBRoleInfo : public Object {
public:
	DBRoleInfo() {
id = 0;
accountId = 0;
name = "";
createTime = 0;
onlinetotaltime = 0;

datastr = "";

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << id);
CHECK(bytes << accountId);
CHECK(bytes << name);
CHECK(bytes << createTime);
CHECK(bytes << onlinetotaltime);
CHECK(bytes << datas);
CHECK(bytes << datastr);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> id);
CHECK(bytes >> accountId);
CHECK(bytes >> name);
CHECK(bytes >> createTime);
CHECK(bytes >> onlinetotaltime);
CHECK(bytes >> datas);
CHECK(bytes >> datastr);

		return true;
	}
public:
uint32 id;
uint32 accountId;
std::string name;
uint32 createTime;
uint32 onlinetotaltime;
BinaryStream datas;
std::string datastr;

};

class Point : public Object {
public:
	Point() {
x = 0;
y = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << x);
CHECK(bytes << y);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> x);
CHECK(bytes >> y);

		return true;
	}
public:
int32 x;
int32 y;

};

class TeamEntityInfo : public Object {
public:
	TeamEntityInfo() {
name = "";
instanceId = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << instanceId);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> instanceId);

		return true;
	}
public:
std::string name;
int32 instanceId;

};

class TeamInfo : public Object {
public:
	TeamInfo() {
teamId = 0;
leaderInsId = 0;


	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << teamId);
CHECK(bytes << leaderInsId);
uint32 teamEntityInfos_Size = (uint32)teamEntityInfos.size();
CHECK(bytes << teamEntityInfos_Size);
for (uint32 teamEntityInfos_i = 0; teamEntityInfos_i < teamEntityInfos_Size; ++teamEntityInfos_i) {
	CHECK(bytes << teamEntityInfos[teamEntityInfos_i]);
}

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> teamId);
CHECK(bytes >> leaderInsId);
uint32 teamEntityInfos_Size = 0;
CHECK(bytes >> teamEntityInfos_Size);
for (uint32 teamEntityInfos_i = 0; teamEntityInfos_i < teamEntityInfos_Size; ++teamEntityInfos_i) {
	TeamEntityInfo teamEntityInfos_info;
	CHECK(bytes >> teamEntityInfos_info);
	teamEntityInfos.push_back(teamEntityInfos_info);
}

		return true;
	}
public:
int32 teamId;
int32 leaderInsId;
std::vector<TeamEntityInfo> teamEntityInfos;

};

class RoomPlayerInfo : public Object {
public:
	RoomPlayerInfo() {
name = "";
userId = 0;
insId = 0;
state = 0;
pos = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << userId);
CHECK(bytes << insId);
CHECK(bytes << state);
CHECK(bytes << pos);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> userId);
CHECK(bytes >> insId);
CHECK(bytes >> state);
CHECK(bytes >> pos);

		return true;
	}
public:
std::string name;
uint32 userId;
uint64 insId;
uint8 state;
uint8 pos;

};

class RoomInfo : public Object {
public:
	RoomInfo() {
roomId = 0;
masterUserId = 0;
password = "";
name = "";
maxCount = 0;


	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << masterUserId);
CHECK(bytes << password);
CHECK(bytes << name);
CHECK(bytes << maxCount);
uint32 roomPlayerInfos_Size = (uint32)roomPlayerInfos.size();
CHECK(bytes << roomPlayerInfos_Size);
for (uint32 roomPlayerInfos_i = 0; roomPlayerInfos_i < roomPlayerInfos_Size; ++roomPlayerInfos_i) {
	CHECK(bytes << roomPlayerInfos[roomPlayerInfos_i]);
}

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> masterUserId);
CHECK(bytes >> password);
CHECK(bytes >> name);
CHECK(bytes >> maxCount);
uint32 roomPlayerInfos_Size = 0;
CHECK(bytes >> roomPlayerInfos_Size);
for (uint32 roomPlayerInfos_i = 0; roomPlayerInfos_i < roomPlayerInfos_Size; ++roomPlayerInfos_i) {
	RoomPlayerInfo roomPlayerInfos_info;
	CHECK(bytes >> roomPlayerInfos_info);
	roomPlayerInfos.push_back(roomPlayerInfos_info);
}

		return true;
	}
public:
uint32 roomId;
uint32 masterUserId;
std::string password;
std::string name;
uint32 maxCount;
std::vector<RoomPlayerInfo> roomPlayerInfos;

};

class GameEntityInfo : public Object {
public:
	GameEntityInfo() {
userId = 0;

userGold = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << userId);
uint32 cards_Size = (uint32)cards.size();
CHECK(bytes << cards_Size);
for (uint32 cards_i = 0; cards_i < cards_Size; ++cards_i) {
	CHECK(bytes << cards[cards_i]);
}
CHECK(bytes << userGold);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> userId);
uint32 cards_Size = 0;
CHECK(bytes >> cards_Size);
for (uint32 cards_i = 0; cards_i < cards_Size; ++cards_i) {
	uint8 cards_info;
	CHECK(bytes >> cards_info);
	cards.push_back(cards_info);
}
CHECK(bytes >> userGold);

		return true;
	}
public:
uint32 userId;
std::vector<uint8> cards;
uint32 userGold;

};

class GameGoldenFlowerInfo : public Object {
public:
	GameGoldenFlowerInfo() {
insId = 0;
roomId = 0;
bankerUserId = 0;
curSpeakUserId = 0;
speakTime = 0;
curUseGold = 0;
curMaxUseGold = 0;
round = 0;


	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << insId);
CHECK(bytes << roomId);
CHECK(bytes << bankerUserId);
CHECK(bytes << curSpeakUserId);
CHECK(bytes << speakTime);
CHECK(bytes << curUseGold);
CHECK(bytes << curMaxUseGold);
CHECK(bytes << round);
uint32 gameEntInfos_Size = (uint32)gameEntInfos.size();
CHECK(bytes << gameEntInfos_Size);
for (uint32 gameEntInfos_i = 0; gameEntInfos_i < gameEntInfos_Size; ++gameEntInfos_i) {
	CHECK(bytes << gameEntInfos[gameEntInfos_i]);
}

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> insId);
CHECK(bytes >> roomId);
CHECK(bytes >> bankerUserId);
CHECK(bytes >> curSpeakUserId);
CHECK(bytes >> speakTime);
CHECK(bytes >> curUseGold);
CHECK(bytes >> curMaxUseGold);
CHECK(bytes >> round);
uint32 gameEntInfos_Size = 0;
CHECK(bytes >> gameEntInfos_Size);
for (uint32 gameEntInfos_i = 0; gameEntInfos_i < gameEntInfos_Size; ++gameEntInfos_i) {
	GameEntityInfo gameEntInfos_info;
	CHECK(bytes >> gameEntInfos_info);
	gameEntInfos.push_back(gameEntInfos_info);
}

		return true;
	}
public:
uint32 insId;
uint32 roomId;
uint32 bankerUserId;
uint32 curSpeakUserId;
uint32 speakTime;
uint32 curUseGold;
uint32 curMaxUseGold;
uint32 round;
std::vector<GameEntityInfo> gameEntInfos;

};

class FriendInfo : public Object {
public:
	FriendInfo() {
userId = 0;
name = "";
groupId = 0;
charId = 0;
state = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << userId);
CHECK(bytes << name);
CHECK(bytes << groupId);
CHECK(bytes << charId);
CHECK(bytes << state);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> userId);
CHECK(bytes >> name);
CHECK(bytes >> groupId);
CHECK(bytes >> charId);
CHECK(bytes >> state);

		return true;
	}
public:
uint32 userId;
std::string name;
uint32 groupId;
uint32 charId;
uint8 state;

};

class ProductInfo : public Object {
public:
	ProductInfo() {
productInsId = 0;
productId = 0;
sellUserId = 0;
buyUserId = 0;
shelvesTime = 0;
unShelvesTime = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << productInsId);
CHECK(bytes << productId);
CHECK(bytes << sellUserId);
CHECK(bytes << buyUserId);
CHECK(bytes << shelvesTime);
CHECK(bytes << unShelvesTime);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> productInsId);
CHECK(bytes >> productId);
CHECK(bytes >> sellUserId);
CHECK(bytes >> buyUserId);
CHECK(bytes >> shelvesTime);
CHECK(bytes >> unShelvesTime);

		return true;
	}
public:
uint32 productInsId;
uint32 productId;
uint32 sellUserId;
uint32 buyUserId;
uint32 shelvesTime;
uint32 unShelvesTime;

};

class MailInfo : public Object {
public:
	MailInfo() {
fromUserName = "";
title = "";
content = "";
datastr = "";
isDown = 0;
isRead = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << fromUserName);
CHECK(bytes << title);
CHECK(bytes << content);
CHECK(bytes << datastr);
CHECK(bytes << isDown);
CHECK(bytes << isRead);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> fromUserName);
CHECK(bytes >> title);
CHECK(bytes >> content);
CHECK(bytes >> datastr);
CHECK(bytes >> isDown);
CHECK(bytes >> isRead);

		return true;
	}
public:
std::string fromUserName;
std::string title;
std::string content;
std::string datastr;
uint8 isDown;
uint8 isRead;

};

class NetFirst : public Packet {
public:
	NetFirst():
	Packet(ID_NetFirst) {


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << sbytes);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> sbytes);

		return true;
	}
public:
BinaryStream sbytes;

};

class NetSessionEnterNotify : public Packet {
public:
	NetSessionEnterNotify():
	Packet(ID_NetSessionEnterNotify) {
host = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << host);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> host);

		return true;
	}
public:
std::string host;

};

class NetSessionLeaveNotify : public Packet {
public:
	NetSessionLeaveNotify():
	Packet(ID_NetSessionLeaveNotify) {
msgCode = 0;
msg = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << msgCode);
CHECK(bytes << msg);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> msgCode);
CHECK(bytes >> msg);

		return true;
	}
public:
int32 msgCode;
std::string msg;

};

class NetPingNotify : public Packet {
public:
	NetPingNotify():
	Packet(ID_NetPingNotify) {
time = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << time);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> time);

		return true;
	}
public:
uint32 time;

};

class NetPingGateNotify : public Packet {
public:
	NetPingGateNotify():
	Packet(ID_NetPingGateNotify) {
time = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << time);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> time);

		return true;
	}
public:
uint32 time;

};

class NetLoginReq : public Packet {
public:
	NetLoginReq():
	Packet(ID_NetLoginReq) {
user = "";
password = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << user);
CHECK(bytes << password);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> user);
CHECK(bytes >> password);

		return true;
	}
public:
std::string user;
std::string password;

};

class NetLoginRes : public Packet {
public:
	NetLoginRes():
	Packet(ID_NetLoginRes) {
result = 0;
guid = 0;
accInfo;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << guid);
CHECK(bytes << accInfo);
uint32 roleInfos_Size = (uint32)roleInfos.size();
CHECK(bytes << roleInfos_Size);
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	CHECK(bytes << roleInfos[roleInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> guid);
CHECK(bytes >> accInfo);
uint32 roleInfos_Size = 0;
CHECK(bytes >> roleInfos_Size);
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	DBRoleInfo roleInfos_info;
	CHECK(bytes >> roleInfos_info);
	roleInfos.push_back(roleInfos_info);
}

		return true;
	}
public:
int32 result;
int64 guid;
DBAccountInfo accInfo;
std::vector<DBRoleInfo> roleInfos;

};

class NetPlayerSaveNotify : public Packet {
public:
	NetPlayerSaveNotify():
	Packet(ID_NetPlayerSaveNotify) {
accountId = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accountId);
uint32 roleInfos_Size = (uint32)roleInfos.size();
CHECK(bytes << roleInfos_Size);
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	CHECK(bytes << roleInfos[roleInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accountId);
uint32 roleInfos_Size = 0;
CHECK(bytes >> roleInfos_Size);
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	DBRoleInfo roleInfos_info;
	CHECK(bytes >> roleInfos_info);
	roleInfos.push_back(roleInfos_info);
}

		return true;
	}
public:
int32 accountId;
std::vector<DBRoleInfo> roleInfos;

};

class NetCreateRoleReq : public Packet {
public:
	NetCreateRoleReq():
	Packet(ID_NetCreateRoleReq) {
accId = 0;
name = "";
charId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accId);
CHECK(bytes << name);
CHECK(bytes << charId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accId);
CHECK(bytes >> name);
CHECK(bytes >> charId);

		return true;
	}
public:
uint32 accId;
std::string name;
uint32 charId;

};

class NetCreateRoleRes : public Packet {
public:
	NetCreateRoleRes():
	Packet(ID_NetCreateRoleRes) {
result = 0;
accId = 0;
roleInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << accId);
CHECK(bytes << roleInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> accId);
CHECK(bytes >> roleInfo);

		return true;
	}
public:
uint8 result;
uint32 accId;
DBRoleInfo roleInfo;

};

class NetSelectRoleReq : public Packet {
public:
	NetSelectRoleReq():
	Packet(ID_NetSelectRoleReq) {
accId = 0;
userId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accId);
CHECK(bytes << userId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accId);
CHECK(bytes >> userId);

		return true;
	}
public:
uint32 accId;
uint32 userId;

};

class NetSelectRoleRes : public Packet {
public:
	NetSelectRoleRes():
	Packet(ID_NetSelectRoleRes) {
result = 0;
accId = 0;
roleInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << accId);
CHECK(bytes << roleInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> accId);
CHECK(bytes >> roleInfo);

		return true;
	}
public:
uint8 result;
uint32 accId;
DBRoleInfo roleInfo;

};

class NetChangeNameReq : public Packet {
public:
	NetChangeNameReq():
	Packet(ID_NetChangeNameReq) {
newName = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << newName);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> newName);

		return true;
	}
public:
std::string newName;

};

class NetChangeNameRes : public Packet {
public:
	NetChangeNameRes():
	Packet(ID_NetChangeNameRes) {
result = 0;
newName = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << newName);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> newName);

		return true;
	}
public:
uint8 result;
std::string newName;

};

class NetGmMsg : public Packet {
public:
	NetGmMsg():
	Packet(ID_NetGmMsg) {
name = "";


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
uint32 gmParams_Size = (uint32)gmParams.size();
CHECK(bytes << gmParams_Size);
for (uint32 gmParams_i = 0; gmParams_i < gmParams_Size; ++gmParams_i) {
	CHECK(bytes << gmParams[gmParams_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
uint32 gmParams_Size = 0;
CHECK(bytes >> gmParams_Size);
for (uint32 gmParams_i = 0; gmParams_i < gmParams_Size; ++gmParams_i) {
	std::string gmParams_info;
	CHECK(bytes >> gmParams_info);
	gmParams.push_back(gmParams_info);
}

		return true;
	}
public:
std::string name;
std::vector<std::string> gmParams;

};

class NetQueryRoleReq : public Packet {
public:
	NetQueryRoleReq():
	Packet(ID_NetQueryRoleReq) {
accountId = 0;
user = "";
roleCount = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accountId);
CHECK(bytes << user);
CHECK(bytes << roleCount);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accountId);
CHECK(bytes >> user);
CHECK(bytes >> roleCount);

		return true;
	}
public:
int32 accountId;
std::string user;
int8 roleCount;

};

class NetQueryRoleRes : public Packet {
public:
	NetQueryRoleRes():
	Packet(ID_NetQueryRoleRes) {
accountId = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accountId);
uint32 roleInfos_Size = (uint32)roleInfos.size();
CHECK(bytes << roleInfos_Size);
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	CHECK(bytes << roleInfos[roleInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accountId);
uint32 roleInfos_Size = 0;
CHECK(bytes >> roleInfos_Size);
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	DBRoleInfo roleInfos_info;
	CHECK(bytes >> roleInfos_info);
	roleInfos.push_back(roleInfos_info);
}

		return true;
	}
public:
int32 accountId;
std::vector<DBRoleInfo> roleInfos;

};

class NetPropertyReq : public Packet {
public:
	NetPropertyReq():
	Packet(ID_NetPropertyReq) {
guid = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);

		return true;
	}
public:
int64 guid;

};

class NetPropertyRes : public Packet {
public:
	NetPropertyRes():
	Packet(ID_NetPropertyRes) {
guid = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << property);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> property);

		return true;
	}
public:
int64 guid;
BinaryStream property;

};

class NetEntityPropertyNotify : public Packet {
public:
	NetEntityPropertyNotify():
	Packet(ID_NetEntityPropertyNotify) {
guid = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << property);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> property);

		return true;
	}
public:
int64 guid;
BinaryStream property;

};

class NetEntityEnterMapNotify : public Packet {
public:
	NetEntityEnterMapNotify():
	Packet(ID_NetEntityEnterMapNotify) {
name = "";
guid = 0;
mapInsId = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << guid);
CHECK(bytes << mapInsId);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> guid);
CHECK(bytes >> mapInsId);
CHECK(bytes >> mapId);

		return true;
	}
public:
std::string name;
int64 guid;
int32 mapInsId;
int32 mapId;

};

class NetEntityLeaveMapNotify : public Packet {
public:
	NetEntityLeaveMapNotify():
	Packet(ID_NetEntityLeaveMapNotify) {
guid = 0;
mapInsId = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << mapInsId);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> mapInsId);
CHECK(bytes >> mapId);

		return true;
	}
public:
int64 guid;
int32 mapInsId;
int32 mapId;

};

class NetPlayerEnterViewNotify : public Packet {
public:
	NetPlayerEnterViewNotify():
	Packet(ID_NetPlayerEnterViewNotify) {
name = "";
guid = 0;
entityType = 0;
mapId = 0;
charId = 0;
pos;
speed = 0.f;
status = 0;
dir = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << guid);
CHECK(bytes << entityType);
CHECK(bytes << mapId);
CHECK(bytes << charId);
CHECK(bytes << pos);
CHECK(bytes << speed);
CHECK(bytes << status);
CHECK(bytes << dir);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> guid);
CHECK(bytes >> entityType);
CHECK(bytes >> mapId);
CHECK(bytes >> charId);
CHECK(bytes >> pos);
CHECK(bytes >> speed);
CHECK(bytes >> status);
CHECK(bytes >> dir);

		return true;
	}
public:
std::string name;
int64 guid;
int32 entityType;
int32 mapId;
int32 charId;
Point pos;
float32 speed;
int32 status;
int32 dir;

};

class NetPlayerLeaveViewNotify : public Packet {
public:
	NetPlayerLeaveViewNotify():
	Packet(ID_NetPlayerLeaveViewNotify) {
guid = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> mapId);

		return true;
	}
public:
int64 guid;
int32 mapId;

};

class NetNpcEnterViewNotify : public Packet {
public:
	NetNpcEnterViewNotify():
	Packet(ID_NetNpcEnterViewNotify) {
name = "";
guid = 0;
entityType = 0;
mapId = 0;
npcId = 0;
pos;
speed = 0.f;
status = 0;
dir = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << guid);
CHECK(bytes << entityType);
CHECK(bytes << mapId);
CHECK(bytes << npcId);
CHECK(bytes << pos);
CHECK(bytes << speed);
CHECK(bytes << status);
CHECK(bytes << dir);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> guid);
CHECK(bytes >> entityType);
CHECK(bytes >> mapId);
CHECK(bytes >> npcId);
CHECK(bytes >> pos);
CHECK(bytes >> speed);
CHECK(bytes >> status);
CHECK(bytes >> dir);

		return true;
	}
public:
std::string name;
int64 guid;
int32 entityType;
int32 mapId;
int32 npcId;
Point pos;
float32 speed;
int32 status;
int32 dir;

};

class NetNpcLeaveViewNotify : public Packet {
public:
	NetNpcLeaveViewNotify():
	Packet(ID_NetNpcLeaveViewNotify) {
guid = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> mapId);

		return true;
	}
public:
int64 guid;
int32 mapId;

};

class NetEntityMoveNotify : public Packet {
public:
	NetEntityMoveNotify():
	Packet(ID_NetEntityMoveNotify) {
guid = 0;
startDir = 0;
x = 0;
y = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << startDir);
CHECK(bytes << x);
CHECK(bytes << y);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> startDir);
CHECK(bytes >> x);
CHECK(bytes >> y);

		return true;
	}
public:
int64 guid;
int8 startDir;
int32 x;
int32 y;

};

class NetEntityMoveToNotify : public Packet {
public:
	NetEntityMoveToNotify():
	Packet(ID_NetEntityMoveToNotify) {
guid = 0;
x = 0;
y = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << x);
CHECK(bytes << y);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> x);
CHECK(bytes >> y);

		return true;
	}
public:
int64 guid;
int32 x;
int32 y;

};

class NetEntityAttackNotify : public Packet {
public:
	NetEntityAttackNotify():
	Packet(ID_NetEntityAttackNotify) {
guid = 0;
index = 0;
tarGuid = 0;
dirPos = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << index);
CHECK(bytes << tarGuid);
CHECK(bytes << dirPos);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> index);
CHECK(bytes >> tarGuid);
CHECK(bytes >> dirPos);

		return true;
	}
public:
int64 guid;
int8 index;
int64 tarGuid;
int8 dirPos;

};

class NetEntityFollowNotify : public Packet {
public:
	NetEntityFollowNotify():
	Packet(ID_NetEntityFollowNotify) {
tarGuid = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << tarGuid);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> tarGuid);

		return true;
	}
public:
int64 tarGuid;

};

class NetEntityCancelFollowNotify : public Packet {
public:
	NetEntityCancelFollowNotify():
	Packet(ID_NetEntityCancelFollowNotify) {
tarGuid = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << tarGuid);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> tarGuid);

		return true;
	}
public:
int64 tarGuid;

};

class NetChatMsgNotify : public Packet {
public:
	NetChatMsgNotify():
	Packet(ID_NetChatMsgNotify) {
from = "";
to = "";
channelType = 0;
chatMsg = "";


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << from);
CHECK(bytes << to);
CHECK(bytes << channelType);
CHECK(bytes << chatMsg);
CHECK(bytes << chatData);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> from);
CHECK(bytes >> to);
CHECK(bytes >> channelType);
CHECK(bytes >> chatMsg);
CHECK(bytes >> chatData);

		return true;
	}
public:
std::string from;
std::string to;
int8 channelType;
std::string chatMsg;
BinaryStream chatData;

};

class NetResponseMsgNotify : public Packet {
public:
	NetResponseMsgNotify():
	Packet(ID_NetResponseMsgNotify) {
msgId = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << msgId);
uint32 msgParams_Size = (uint32)msgParams.size();
CHECK(bytes << msgParams_Size);
for (uint32 msgParams_i = 0; msgParams_i < msgParams_Size; ++msgParams_i) {
	CHECK(bytes << msgParams[msgParams_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> msgId);
uint32 msgParams_Size = 0;
CHECK(bytes >> msgParams_Size);
for (uint32 msgParams_i = 0; msgParams_i < msgParams_Size; ++msgParams_i) {
	std::string msgParams_info;
	CHECK(bytes >> msgParams_info);
	msgParams.push_back(msgParams_info);
}

		return true;
	}
public:
int32 msgId;
std::vector<std::string> msgParams;

};

class NetCreateTeamReq : public Packet {
public:
	NetCreateTeamReq():
	Packet(ID_NetCreateTeamReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetCreateTeamRes : public Packet {
public:
	NetCreateTeamRes():
	Packet(ID_NetCreateTeamRes) {
teamInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << teamInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> teamInfo);

		return true;
	}
public:
TeamInfo teamInfo;

};

class NetOrganizeTeamReq : public Packet {
public:
	NetOrganizeTeamReq():
	Packet(ID_NetOrganizeTeamReq) {
name = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);

		return true;
	}
public:
std::string name;

};

class NetOrganizeTeamRes : public Packet {
public:
	NetOrganizeTeamRes():
	Packet(ID_NetOrganizeTeamRes) {
name = "";
isJoin = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << isJoin);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> isJoin);

		return true;
	}
public:
std::string name;
int8 isJoin;

};

class NetAgreeTeamReq : public Packet {
public:
	NetAgreeTeamReq():
	Packet(ID_NetAgreeTeamReq) {
name = "";
isJoin = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << isJoin);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> isJoin);

		return true;
	}
public:
std::string name;
int8 isJoin;

};

class NetTeamInfoRes : public Packet {
public:
	NetTeamInfoRes():
	Packet(ID_NetTeamInfoRes) {
teamInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << teamInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> teamInfo);

		return true;
	}
public:
TeamInfo teamInfo;

};

class NetTeamListReq : public Packet {
public:
	NetTeamListReq():
	Packet(ID_NetTeamListReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetTeamListRes : public Packet {
public:
	NetTeamListRes():
	Packet(ID_NetTeamListRes) {


	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 teamInfos_Size = (uint32)teamInfos.size();
CHECK(bytes << teamInfos_Size);
for (uint32 teamInfos_i = 0; teamInfos_i < teamInfos_Size; ++teamInfos_i) {
	CHECK(bytes << teamInfos[teamInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 teamInfos_Size = 0;
CHECK(bytes >> teamInfos_Size);
for (uint32 teamInfos_i = 0; teamInfos_i < teamInfos_Size; ++teamInfos_i) {
	TeamInfo teamInfos_info;
	CHECK(bytes >> teamInfos_info);
	teamInfos.push_back(teamInfos_info);
}

		return true;
	}
public:
std::vector<TeamInfo> teamInfos;

};

class NetCreateRoomReq : public Packet {
public:
	NetCreateRoomReq():
	Packet(ID_NetCreateRoomReq) {
name = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);

		return true;
	}
public:
std::string name;

};

class NetCreateRoomRes : public Packet {
public:
	NetCreateRoomRes():
	Packet(ID_NetCreateRoomRes) {
roomInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomInfo);

		return true;
	}
public:
RoomInfo roomInfo;

};

class NetEnterRoomReq : public Packet {
public:
	NetEnterRoomReq():
	Packet(ID_NetEnterRoomReq) {
roomId = 0;
password = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << password);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> password);

		return true;
	}
public:
uint32 roomId;
std::string password;

};

class NetEnterRoomRes : public Packet {
public:
	NetEnterRoomRes():
	Packet(ID_NetEnterRoomRes) {
roomInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomInfo);

		return true;
	}
public:
RoomInfo roomInfo;

};

class NetEnterRoomNotify : public Packet {
public:
	NetEnterRoomNotify():
	Packet(ID_NetEnterRoomNotify) {
roomId = 0;
roomPlayerInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << roomPlayerInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> roomPlayerInfo);

		return true;
	}
public:
uint32 roomId;
RoomPlayerInfo roomPlayerInfo;

};

class NetLeaveRoomReq : public Packet {
public:
	NetLeaveRoomReq():
	Packet(ID_NetLeaveRoomReq) {
roomId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);

		return true;
	}
public:
uint32 roomId;

};

class NetLeaveRoomRes : public Packet {
public:
	NetLeaveRoomRes():
	Packet(ID_NetLeaveRoomRes) {
roomId = 0;
userId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << userId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> userId);

		return true;
	}
public:
uint32 roomId;
uint32 userId;

};

class NetChangeRoomMasterReq : public Packet {
public:
	NetChangeRoomMasterReq():
	Packet(ID_NetChangeRoomMasterReq) {
roomId = 0;
userId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << userId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> userId);

		return true;
	}
public:
uint32 roomId;
uint32 userId;

};

class NetChangeRoomMasterRes : public Packet {
public:
	NetChangeRoomMasterRes():
	Packet(ID_NetChangeRoomMasterRes) {
roomId = 0;
masterUserId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << masterUserId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> masterUserId);

		return true;
	}
public:
uint32 roomId;
uint32 masterUserId;

};

class NetRoomListReq : public Packet {
public:
	NetRoomListReq():
	Packet(ID_NetRoomListReq) {
start = 0;
count = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << start);
CHECK(bytes << count);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> start);
CHECK(bytes >> count);

		return true;
	}
public:
uint32 start;
uint32 count;

};

class NetRoomListRes : public Packet {
public:
	NetRoomListRes():
	Packet(ID_NetRoomListRes) {


	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 roomInfoInfos_Size = (uint32)roomInfoInfos.size();
CHECK(bytes << roomInfoInfos_Size);
for (uint32 roomInfoInfos_i = 0; roomInfoInfos_i < roomInfoInfos_Size; ++roomInfoInfos_i) {
	CHECK(bytes << roomInfoInfos[roomInfoInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 roomInfoInfos_Size = 0;
CHECK(bytes >> roomInfoInfos_Size);
for (uint32 roomInfoInfos_i = 0; roomInfoInfos_i < roomInfoInfos_Size; ++roomInfoInfos_i) {
	RoomInfo roomInfoInfos_info;
	CHECK(bytes >> roomInfoInfos_info);
	roomInfoInfos.push_back(roomInfoInfos_info);
}

		return true;
	}
public:
std::vector<RoomInfo> roomInfoInfos;

};

class NetRoomReadyReq : public Packet {
public:
	NetRoomReadyReq():
	Packet(ID_NetRoomReadyReq) {
isReady = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << isReady);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> isReady);

		return true;
	}
public:
uint8 isReady;

};

class NetRoomReadyRes : public Packet {
public:
	NetRoomReadyRes():
	Packet(ID_NetRoomReadyRes) {
userId = 0;
result = 0;
isReady = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << userId);
CHECK(bytes << result);
CHECK(bytes << isReady);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> userId);
CHECK(bytes >> result);
CHECK(bytes >> isReady);

		return true;
	}
public:
uint32 userId;
uint8 result;
uint8 isReady;

};

class NetRoomStartGameReq : public Packet {
public:
	NetRoomStartGameReq():
	Packet(ID_NetRoomStartGameReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetRoomStartGameRes : public Packet {
public:
	NetRoomStartGameRes():
	Packet(ID_NetRoomStartGameRes) {
result = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);

		return true;
	}
public:
uint8 result;

};

class NetRoomInfoNotify : public Packet {
public:
	NetRoomInfoNotify():
	Packet(ID_NetRoomInfoNotify) {
roomInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomInfo);

		return true;
	}
public:
RoomInfo roomInfo;

};

class NetGameStartNotify : public Packet {
public:
	NetGameStartNotify():
	Packet(ID_NetGameStartNotify) {
info;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << info);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> info);

		return true;
	}
public:
GameGoldenFlowerInfo info;

};

class NetGameCloseNotify : public Packet {
public:
	NetGameCloseNotify():
	Packet(ID_NetGameCloseNotify) {
winUserId = 0;
winGold = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << winUserId);
CHECK(bytes << winGold);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> winUserId);
CHECK(bytes >> winGold);

		return true;
	}
public:
uint32 winUserId;
uint32 winGold;

};

class NetGameInfoNotify : public Packet {
public:
	NetGameInfoNotify():
	Packet(ID_NetGameInfoNotify) {
info;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << info);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> info);

		return true;
	}
public:
GameGoldenFlowerInfo info;

};

class NetGameOperateSeeReq : public Packet {
public:
	NetGameOperateSeeReq():
	Packet(ID_NetGameOperateSeeReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetGameOperateSeeRes : public Packet {
public:
	NetGameOperateSeeRes():
	Packet(ID_NetGameOperateSeeRes) {
result = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
uint32 cards_Size = (uint32)cards.size();
CHECK(bytes << cards_Size);
for (uint32 cards_i = 0; cards_i < cards_Size; ++cards_i) {
	CHECK(bytes << cards[cards_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
uint32 cards_Size = 0;
CHECK(bytes >> cards_Size);
for (uint32 cards_i = 0; cards_i < cards_Size; ++cards_i) {
	uint8 cards_info;
	CHECK(bytes >> cards_info);
	cards.push_back(cards_info);
}

		return true;
	}
public:
uint8 result;
std::vector<uint8> cards;

};

class NetGameOperateGiveupReq : public Packet {
public:
	NetGameOperateGiveupReq():
	Packet(ID_NetGameOperateGiveupReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetGameOperateGiveupRes : public Packet {
public:
	NetGameOperateGiveupRes():
	Packet(ID_NetGameOperateGiveupRes) {
userId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << userId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> userId);

		return true;
	}
public:
uint32 userId;

};

class NetGameOperateChipinReq : public Packet {
public:
	NetGameOperateChipinReq():
	Packet(ID_NetGameOperateChipinReq) {
gold = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << gold);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> gold);

		return true;
	}
public:
uint32 gold;

};

class NetGameOperateChipinRes : public Packet {
public:
	NetGameOperateChipinRes():
	Packet(ID_NetGameOperateChipinRes) {
result = 0;
userId = 0;
gold = 0;
state = 0;
nextSpeakUserId = 0;
speakTime = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << userId);
CHECK(bytes << gold);
CHECK(bytes << state);
CHECK(bytes << nextSpeakUserId);
CHECK(bytes << speakTime);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> userId);
CHECK(bytes >> gold);
CHECK(bytes >> state);
CHECK(bytes >> nextSpeakUserId);
CHECK(bytes >> speakTime);

		return true;
	}
public:
uint8 result;
uint32 userId;
uint32 gold;
uint8 state;
uint32 nextSpeakUserId;
uint32 speakTime;

};

class NetGameOperateCallReq : public Packet {
public:
	NetGameOperateCallReq():
	Packet(ID_NetGameOperateCallReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetGameOperateCallRes : public Packet {
public:
	NetGameOperateCallRes():
	Packet(ID_NetGameOperateCallRes) {
result = 0;
userId = 0;
gold = 0;
state = 0;
nextSpeakUserId = 0;
speakTime = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << userId);
CHECK(bytes << gold);
CHECK(bytes << state);
CHECK(bytes << nextSpeakUserId);
CHECK(bytes << speakTime);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> userId);
CHECK(bytes >> gold);
CHECK(bytes >> state);
CHECK(bytes >> nextSpeakUserId);
CHECK(bytes >> speakTime);

		return true;
	}
public:
uint8 result;
uint32 userId;
uint32 gold;
uint8 state;
uint32 nextSpeakUserId;
uint32 speakTime;

};

class NetGameOperateCompareReq : public Packet {
public:
	NetGameOperateCompareReq():
	Packet(ID_NetGameOperateCompareReq) {
tarUserId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << tarUserId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> tarUserId);

		return true;
	}
public:
uint32 tarUserId;

};

class NetGameOperateCompareRes : public Packet {
public:
	NetGameOperateCompareRes():
	Packet(ID_NetGameOperateCompareRes) {
result = 0;
userId = 0;

gold = 0;
state = 0;
tarUserId = 0;

tarState = 0;
nextSpeakUserId = 0;
speakTime = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << userId);
uint32 cards_Size = (uint32)cards.size();
CHECK(bytes << cards_Size);
for (uint32 cards_i = 0; cards_i < cards_Size; ++cards_i) {
	CHECK(bytes << cards[cards_i]);
}
CHECK(bytes << gold);
CHECK(bytes << state);
CHECK(bytes << tarUserId);
uint32 tarCards_Size = (uint32)tarCards.size();
CHECK(bytes << tarCards_Size);
for (uint32 tarCards_i = 0; tarCards_i < tarCards_Size; ++tarCards_i) {
	CHECK(bytes << tarCards[tarCards_i]);
}
CHECK(bytes << tarState);
CHECK(bytes << nextSpeakUserId);
CHECK(bytes << speakTime);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> userId);
uint32 cards_Size = 0;
CHECK(bytes >> cards_Size);
for (uint32 cards_i = 0; cards_i < cards_Size; ++cards_i) {
	uint8 cards_info;
	CHECK(bytes >> cards_info);
	cards.push_back(cards_info);
}
CHECK(bytes >> gold);
CHECK(bytes >> state);
CHECK(bytes >> tarUserId);
uint32 tarCards_Size = 0;
CHECK(bytes >> tarCards_Size);
for (uint32 tarCards_i = 0; tarCards_i < tarCards_Size; ++tarCards_i) {
	uint8 tarCards_info;
	CHECK(bytes >> tarCards_info);
	tarCards.push_back(tarCards_info);
}
CHECK(bytes >> tarState);
CHECK(bytes >> nextSpeakUserId);
CHECK(bytes >> speakTime);

		return true;
	}
public:
uint8 result;
uint32 userId;
std::vector<uint8> cards;
uint32 gold;
uint8 state;
uint32 tarUserId;
std::vector<uint8> tarCards;
uint8 tarState;
uint32 nextSpeakUserId;
uint32 speakTime;

};

class NetAddFriendReq : public Packet {
public:
	NetAddFriendReq():
	Packet(ID_NetAddFriendReq) {
tarName = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << tarName);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> tarName);

		return true;
	}
public:
std::string tarName;

};

class NetAddFriendRes : public Packet {
public:
	NetAddFriendRes():
	Packet(ID_NetAddFriendRes) {
result = 0;
tarName = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << tarName);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> tarName);

		return true;
	}
public:
uint8 result;
std::string tarName;

};

class NetAddFriendNotify : public Packet {
public:
	NetAddFriendNotify():
	Packet(ID_NetAddFriendNotify) {
friendInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << friendInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> friendInfo);

		return true;
	}
public:
FriendInfo friendInfo;

};

class NetFriendListReq : public Packet {
public:
	NetFriendListReq():
	Packet(ID_NetFriendListReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetFriendListRes : public Packet {
public:
	NetFriendListRes():
	Packet(ID_NetFriendListRes) {




	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 gruopIds_Size = (uint32)gruopIds.size();
CHECK(bytes << gruopIds_Size);
for (uint32 gruopIds_i = 0; gruopIds_i < gruopIds_Size; ++gruopIds_i) {
	CHECK(bytes << gruopIds[gruopIds_i]);
}
uint32 gruopNames_Size = (uint32)gruopNames.size();
CHECK(bytes << gruopNames_Size);
for (uint32 gruopNames_i = 0; gruopNames_i < gruopNames_Size; ++gruopNames_i) {
	CHECK(bytes << gruopNames[gruopNames_i]);
}
uint32 friendInfos_Size = (uint32)friendInfos.size();
CHECK(bytes << friendInfos_Size);
for (uint32 friendInfos_i = 0; friendInfos_i < friendInfos_Size; ++friendInfos_i) {
	CHECK(bytes << friendInfos[friendInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 gruopIds_Size = 0;
CHECK(bytes >> gruopIds_Size);
for (uint32 gruopIds_i = 0; gruopIds_i < gruopIds_Size; ++gruopIds_i) {
	uint32 gruopIds_info;
	CHECK(bytes >> gruopIds_info);
	gruopIds.push_back(gruopIds_info);
}
uint32 gruopNames_Size = 0;
CHECK(bytes >> gruopNames_Size);
for (uint32 gruopNames_i = 0; gruopNames_i < gruopNames_Size; ++gruopNames_i) {
	std::string gruopNames_info;
	CHECK(bytes >> gruopNames_info);
	gruopNames.push_back(gruopNames_info);
}
uint32 friendInfos_Size = 0;
CHECK(bytes >> friendInfos_Size);
for (uint32 friendInfos_i = 0; friendInfos_i < friendInfos_Size; ++friendInfos_i) {
	FriendInfo friendInfos_info;
	CHECK(bytes >> friendInfos_info);
	friendInfos.push_back(friendInfos_info);
}

		return true;
	}
public:
std::vector<uint32> gruopIds;
std::vector<std::string> gruopNames;
std::vector<FriendInfo> friendInfos;

};

class NetSellProductReq : public Packet {
public:
	NetSellProductReq():
	Packet(ID_NetSellProductReq) {
productId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << productId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> productId);

		return true;
	}
public:
uint32 productId;

};

class NetSellProductRes : public Packet {
public:
	NetSellProductRes():
	Packet(ID_NetSellProductRes) {
result = 0;
productInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << productInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> productInfo);

		return true;
	}
public:
uint32 result;
ProductInfo productInfo;

};

class NetProductListReq : public Packet {
public:
	NetProductListReq():
	Packet(ID_NetProductListReq) {
index = 0;
count = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << index);
CHECK(bytes << count);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> index);
CHECK(bytes >> count);

		return true;
	}
public:
uint32 index;
uint32 count;

};

class NetProductListRes : public Packet {
public:
	NetProductListRes():
	Packet(ID_NetProductListRes) {


	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 productInfos_Size = (uint32)productInfos.size();
CHECK(bytes << productInfos_Size);
for (uint32 productInfos_i = 0; productInfos_i < productInfos_Size; ++productInfos_i) {
	CHECK(bytes << productInfos[productInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 productInfos_Size = 0;
CHECK(bytes >> productInfos_Size);
for (uint32 productInfos_i = 0; productInfos_i < productInfos_Size; ++productInfos_i) {
	ProductInfo productInfos_info;
	CHECK(bytes >> productInfos_info);
	productInfos.push_back(productInfos_info);
}

		return true;
	}
public:
std::vector<ProductInfo> productInfos;

};

class NetMailListNotify : public Packet {
public:
	NetMailListNotify():
	Packet(ID_NetMailListNotify) {


	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 mailInfos_Size = (uint32)mailInfos.size();
CHECK(bytes << mailInfos_Size);
for (uint32 mailInfos_i = 0; mailInfos_i < mailInfos_Size; ++mailInfos_i) {
	CHECK(bytes << mailInfos[mailInfos_i]);
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 mailInfos_Size = 0;
CHECK(bytes >> mailInfos_Size);
for (uint32 mailInfos_i = 0; mailInfos_i < mailInfos_Size; ++mailInfos_i) {
	MailInfo mailInfos_info;
	CHECK(bytes >> mailInfos_info);
	mailInfos.push_back(mailInfos_info);
}

		return true;
	}
public:
std::vector<MailInfo> mailInfos;

};

class NetEnd : public Packet {
public:
	NetEnd():
	Packet(ID_NetEnd) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};



REGISTER_PACKET_HELPER(ID_NetFirst, NetFirst);
REGISTER_PACKET_HELPER(ID_NetSessionEnterNotify, NetSessionEnterNotify);
REGISTER_PACKET_HELPER(ID_NetSessionLeaveNotify, NetSessionLeaveNotify);
REGISTER_PACKET_HELPER(ID_NetPingNotify, NetPingNotify);
REGISTER_PACKET_HELPER(ID_NetPingGateNotify, NetPingGateNotify);
REGISTER_PACKET_HELPER(ID_NetLoginReq, NetLoginReq);
REGISTER_PACKET_HELPER(ID_NetLoginRes, NetLoginRes);
REGISTER_PACKET_HELPER(ID_NetPlayerSaveNotify, NetPlayerSaveNotify);
REGISTER_PACKET_HELPER(ID_NetCreateRoleReq, NetCreateRoleReq);
REGISTER_PACKET_HELPER(ID_NetCreateRoleRes, NetCreateRoleRes);
REGISTER_PACKET_HELPER(ID_NetSelectRoleReq, NetSelectRoleReq);
REGISTER_PACKET_HELPER(ID_NetSelectRoleRes, NetSelectRoleRes);
REGISTER_PACKET_HELPER(ID_NetChangeNameReq, NetChangeNameReq);
REGISTER_PACKET_HELPER(ID_NetChangeNameRes, NetChangeNameRes);
REGISTER_PACKET_HELPER(ID_NetGmMsg, NetGmMsg);
REGISTER_PACKET_HELPER(ID_NetQueryRoleReq, NetQueryRoleReq);
REGISTER_PACKET_HELPER(ID_NetQueryRoleRes, NetQueryRoleRes);
REGISTER_PACKET_HELPER(ID_NetPropertyReq, NetPropertyReq);
REGISTER_PACKET_HELPER(ID_NetPropertyRes, NetPropertyRes);
REGISTER_PACKET_HELPER(ID_NetEntityPropertyNotify, NetEntityPropertyNotify);
REGISTER_PACKET_HELPER(ID_NetEntityEnterMapNotify, NetEntityEnterMapNotify);
REGISTER_PACKET_HELPER(ID_NetEntityLeaveMapNotify, NetEntityLeaveMapNotify);
REGISTER_PACKET_HELPER(ID_NetPlayerEnterViewNotify, NetPlayerEnterViewNotify);
REGISTER_PACKET_HELPER(ID_NetPlayerLeaveViewNotify, NetPlayerLeaveViewNotify);
REGISTER_PACKET_HELPER(ID_NetNpcEnterViewNotify, NetNpcEnterViewNotify);
REGISTER_PACKET_HELPER(ID_NetNpcLeaveViewNotify, NetNpcLeaveViewNotify);
REGISTER_PACKET_HELPER(ID_NetEntityMoveNotify, NetEntityMoveNotify);
REGISTER_PACKET_HELPER(ID_NetEntityMoveToNotify, NetEntityMoveToNotify);
REGISTER_PACKET_HELPER(ID_NetEntityAttackNotify, NetEntityAttackNotify);
REGISTER_PACKET_HELPER(ID_NetEntityFollowNotify, NetEntityFollowNotify);
REGISTER_PACKET_HELPER(ID_NetEntityCancelFollowNotify, NetEntityCancelFollowNotify);
REGISTER_PACKET_HELPER(ID_NetChatMsgNotify, NetChatMsgNotify);
REGISTER_PACKET_HELPER(ID_NetResponseMsgNotify, NetResponseMsgNotify);
REGISTER_PACKET_HELPER(ID_NetCreateTeamReq, NetCreateTeamReq);
REGISTER_PACKET_HELPER(ID_NetCreateTeamRes, NetCreateTeamRes);
REGISTER_PACKET_HELPER(ID_NetOrganizeTeamReq, NetOrganizeTeamReq);
REGISTER_PACKET_HELPER(ID_NetOrganizeTeamRes, NetOrganizeTeamRes);
REGISTER_PACKET_HELPER(ID_NetAgreeTeamReq, NetAgreeTeamReq);
REGISTER_PACKET_HELPER(ID_NetTeamInfoRes, NetTeamInfoRes);
REGISTER_PACKET_HELPER(ID_NetTeamListReq, NetTeamListReq);
REGISTER_PACKET_HELPER(ID_NetTeamListRes, NetTeamListRes);
REGISTER_PACKET_HELPER(ID_NetCreateRoomReq, NetCreateRoomReq);
REGISTER_PACKET_HELPER(ID_NetCreateRoomRes, NetCreateRoomRes);
REGISTER_PACKET_HELPER(ID_NetEnterRoomReq, NetEnterRoomReq);
REGISTER_PACKET_HELPER(ID_NetEnterRoomRes, NetEnterRoomRes);
REGISTER_PACKET_HELPER(ID_NetEnterRoomNotify, NetEnterRoomNotify);
REGISTER_PACKET_HELPER(ID_NetLeaveRoomReq, NetLeaveRoomReq);
REGISTER_PACKET_HELPER(ID_NetLeaveRoomRes, NetLeaveRoomRes);
REGISTER_PACKET_HELPER(ID_NetChangeRoomMasterReq, NetChangeRoomMasterReq);
REGISTER_PACKET_HELPER(ID_NetChangeRoomMasterRes, NetChangeRoomMasterRes);
REGISTER_PACKET_HELPER(ID_NetRoomListReq, NetRoomListReq);
REGISTER_PACKET_HELPER(ID_NetRoomListRes, NetRoomListRes);
REGISTER_PACKET_HELPER(ID_NetRoomReadyReq, NetRoomReadyReq);
REGISTER_PACKET_HELPER(ID_NetRoomReadyRes, NetRoomReadyRes);
REGISTER_PACKET_HELPER(ID_NetRoomStartGameReq, NetRoomStartGameReq);
REGISTER_PACKET_HELPER(ID_NetRoomStartGameRes, NetRoomStartGameRes);
REGISTER_PACKET_HELPER(ID_NetRoomInfoNotify, NetRoomInfoNotify);
REGISTER_PACKET_HELPER(ID_NetGameStartNotify, NetGameStartNotify);
REGISTER_PACKET_HELPER(ID_NetGameCloseNotify, NetGameCloseNotify);
REGISTER_PACKET_HELPER(ID_NetGameInfoNotify, NetGameInfoNotify);
REGISTER_PACKET_HELPER(ID_NetGameOperateSeeReq, NetGameOperateSeeReq);
REGISTER_PACKET_HELPER(ID_NetGameOperateSeeRes, NetGameOperateSeeRes);
REGISTER_PACKET_HELPER(ID_NetGameOperateGiveupReq, NetGameOperateGiveupReq);
REGISTER_PACKET_HELPER(ID_NetGameOperateGiveupRes, NetGameOperateGiveupRes);
REGISTER_PACKET_HELPER(ID_NetGameOperateChipinReq, NetGameOperateChipinReq);
REGISTER_PACKET_HELPER(ID_NetGameOperateChipinRes, NetGameOperateChipinRes);
REGISTER_PACKET_HELPER(ID_NetGameOperateCallReq, NetGameOperateCallReq);
REGISTER_PACKET_HELPER(ID_NetGameOperateCallRes, NetGameOperateCallRes);
REGISTER_PACKET_HELPER(ID_NetGameOperateCompareReq, NetGameOperateCompareReq);
REGISTER_PACKET_HELPER(ID_NetGameOperateCompareRes, NetGameOperateCompareRes);
REGISTER_PACKET_HELPER(ID_NetAddFriendReq, NetAddFriendReq);
REGISTER_PACKET_HELPER(ID_NetAddFriendRes, NetAddFriendRes);
REGISTER_PACKET_HELPER(ID_NetAddFriendNotify, NetAddFriendNotify);
REGISTER_PACKET_HELPER(ID_NetFriendListReq, NetFriendListReq);
REGISTER_PACKET_HELPER(ID_NetFriendListRes, NetFriendListRes);
REGISTER_PACKET_HELPER(ID_NetSellProductReq, NetSellProductReq);
REGISTER_PACKET_HELPER(ID_NetSellProductRes, NetSellProductRes);
REGISTER_PACKET_HELPER(ID_NetProductListReq, NetProductListReq);
REGISTER_PACKET_HELPER(ID_NetProductListRes, NetProductListRes);
REGISTER_PACKET_HELPER(ID_NetMailListNotify, NetMailListNotify);
REGISTER_PACKET_HELPER(ID_NetEnd, NetEnd);

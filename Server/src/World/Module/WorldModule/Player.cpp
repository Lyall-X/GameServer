#include "stdafx.h"

Player::Player() :
Entity(ET_Player),
accountId(0),
mJson()
{
	mOnlineTimer = 0;
	mOfflineTimer = 0;
	mOnline = false;

	addProperty(new PlayerProperty);
}

Player::~Player()
{
}

bool Player::Initialize()
{
	Entity::Initialize();
	mTimer.addEventListener((EventCallback)&Player::onTimerSaveDB, this, 60.f);
	return true;
}

bool Player::Update(float time, float delay)
{
	Entity::Update(time, delay);
	return true;
}

bool Player::Destroy()
{
	sApp.doPlayerLeaveWorld(this);
	sWorld.removePlayer(getAccId());
	return true;
}

void Player::sendPacket(Packet& packet)
{
	if (!ssn)
		return;
	ssn->sendPacketToWorld(packet);
}

void Player::sendBuffer(void* data, int32 count)
{
	if (!ssn)
		return;
	ssn->sendBufferToWorld(data, count);
}

void Player::sendPacketToView(Packet& packet)
{
	if (!getMap())
		return;

	EntitySet& setEntity = GetVisual().GetEntitySet();
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (auto itr : setEntity)
		itr->sendBuffer(in.datas(), in.wpos());
}

void Player::sendPacketToMap(Packet& packet)
{
	if (mMap == NULL)
		return;
	mMap->sendPacketToAll(packet);
}

void Player::sendPacketToWorld(Packet& packet)
{
	sWorld.sendPacketToAll(packet);
}

void Player::sendPacketToTeam(Packet& packet)
{
	sTeam.sendPacketToTeam(packet, this);
}

void Player::sendPacketToRoom(Packet& packet)
{
	sRoom.sendPacketToRoom(packet, this);
}

void Player::sendPacketToTarget(Packet& packet, Entity* tar)
{
	tar->sendPacket(packet);
}

void Player::sendRespnoseMsg(int32 msgId, std::vector<std::string>* msgParams /* = NULL */)
{
	NetResponseMsgNotify nfy;
	nfy.msgId = msgId;
	if (msgParams)
		nfy.msgParams = *msgParams;

	LOG_INFO("%s: %s", getName(), INSTANCE(ConfigManager).getMsg(msgId).c_str());

	sendPacket(nfy);
}

void Player::sendPacketToMsg(EnumChannel ec, const std::string& msg)
{
	sWorld.sendPacketToMsg((EnumChannel)ec, msg, this);
}

void Player::sendPacketToMsg(const std::string& msg)
{
	sWorld.sendPacketToMsg(EC_TARGET, msg, this);
}

bool Player::CanDestroy()
{
	if (mOnline == false && !sRoom.FindRoom(getUserId()))
		return true;
	return false;
}

void Player::bindSession(Session* ssn)
{
	ssn->setPlayer(this);
	setSession(ssn);
}

void Player::unbindSession()
{
	if (ssn)
		ssn->setPlayer(NULL);
	setSession(NULL);
}

Json::Value& Player::GetJson()
{
	return mJson;
}

void Player::DoCreateCharacter(Dictionary& dict, DBRoleInfo& dbRoleInfo)
{
	OnCreate();
	Dictionary aPropertyDict;

	uint32 charId = INSTANCE(ConfigManager).getCharJsonRandId();
	float32 fSpeed = 0.f;
	uint32 MaxHp = 0;
	uint32 MaxMp = 0;

	CharJson* charJson = INSTANCE(ConfigManager).getCharJson(charId);
	if (charJson) {
		setSex(charJson->Sex);
		PropertyJson* propertyJson = INSTANCE(ConfigManager).getPropertyJson(charJson->PropertyId);
		if (propertyJson) {
			fSpeed = propertyJson->Speed;
			MaxHp = propertyJson->MaxHp;
			MaxMp = propertyJson->MaxMp;
		}
	}

	aPropertyDict.Add(ep_name, dbRoleInfo.name);
	aPropertyDict.Add(ep_CharId, charId);
	aPropertyDict.Add(ep_mapId, (uint32)1);
	aPropertyDict.Add(ep_speed, fSpeed);
	aPropertyDict.Add(ep_posX, (int32)0);
	aPropertyDict.Add(ep_posY, (int32)0);
	aPropertyDict.Add(ep_dirPos, (int8)D_UP);
	aPropertyDict.Add(ep_Level, (uint8)1);
	aPropertyDict.Add(ep_Hp, MaxHp);
	aPropertyDict.Add(ep_Mp, MaxMp);

	BinaryStream bytes;
	bytes << aPropertyDict;
	dict.Add("Property", bytes);

	Dictionary saveDict;
	sApp.doPlayerSave(this, saveDict);
}

bool Player::onSaveBegin(Dictionary& dict)
{
	Json::Value& root = GetJson();
	root.clear();
	return true;
}

bool Player::onSave(Dictionary& dict)
{
	onSavejson(dict);
	onSaveProperty(dict);

	LOG_DEBUG(LogSystem::csl_color_green, "onSave[%s]", getName());
	for (auto itr : dict.MapVariant())
	{
		const Variant& var = itr.first;
		const Variant& second = itr.second;

		if (var.type() == Variant::TypeString)
			LOG_INFO("type:[%s] value:[%s]", var.valueString(), second.toString().c_str());
		else
			LOG_INFO("type:[%s] value:[%s]", Property::GetPropertyName(var.valueInt32()).c_str(), second.toString().c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, "onSave");
	return true;
}

bool Player::onSaveEnd(Dictionary& dict)
{
	return true;
}

bool Player::onLoad(Dictionary& dict)
{
	onLoadJson(dict);
	onLoadProperty(dict);

	LOG_DEBUG(LogSystem::csl_color_green, "onLoad[%s]", getName());
	for (auto itr : dict.MapVariant())
	{
		const Variant& var = itr.first;
		const Variant& second = itr.second;

		if (var.type() == Variant::TypeString)
			LOG_INFO("type:[%s] value:[%s]", var.valueString(), second.toString().c_str());
		else
			LOG_INFO("type:[%s] value:[%s]", Property::GetPropertyName(var.valueInt32()).c_str(), second.toString().c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, "onLoad");

	return true;
}

bool Player::onLoadJson(Dictionary& dict)
{
	std::string jsonstr;
	if (dict.ContainsKey("datastr"))
		jsonstr = dict["datastr"].valueString();
	Json::Reader jsonReader;
	Json::Value& root = GetJson();
	root.clear();

	if (!jsonReader.parse(jsonstr, root))
		return false;
	onLoadJson(root);
	return true;
}

bool Player::onLoadJson(Json::Value& root)
{
	Json::Value userJson = root["user"]; 
	uint32 offline_time = userJson["offline"].asUInt();
	std::string name = userJson["name"].asString();

	uint64 dbytes = userJson["DownloadBytes"].asUInt();
	uint64 ubytes = userJson["UploadBytes"].asUInt();

	Json::Value& oldNameJson = userJson["oldNames"];
	for (uint32 i = 0; i < oldNameJson.size(); ++i) {
		AddOldName(oldNameJson[i].asCString());
	}

	setDownloadBytes(dbytes);
	setUploadBytes(ubytes);
	SetOfflineTimer(offline_time);

	return true;
}

bool Player::onLoadProperty(Dictionary& dict)
{
	Variant* varBytes = dict.GetVariant("Property");
	if (varBytes == NULL) return true;
	BinaryStream bytes = (BinaryStream&)varBytes->valueBytes();
	Dictionary aProperyDict;
	bytes >> aProperyDict;

	setName(aProperyDict[ep_name].valueString());
	setCharId(aProperyDict[ep_CharId].valueUint32());
	setSpeed(aProperyDict[ep_speed].valueFloat32());
	setCellX(aProperyDict[ep_posX].valueInt32());
	setCellTarX(getCellX());
	setCellY(aProperyDict[ep_posY].valueInt32());
	setCellTarY(getCellY());
	setDirPosition(aProperyDict[ep_dirPos].valueInt8());

	sProperty.setLevel(this, aProperyDict[ep_Level].valueUint8());
	sProperty.setHp(this, aProperyDict[ep_Hp].valueUint32());
	sProperty.setMp(this, aProperyDict[ep_Mp].valueUint32());

	sProperty.setMaxHp(this, sProperty.CalculateMaxHp(this));
	sProperty.setMaxMp(this, sProperty.CalculateMaxMp(this));
	sProperty.setAttack(this, sProperty.CalculateAttack(this));
	sProperty.setDefense(this, sProperty.CalculateDefense(this));
	return true;
}

bool Player::onSavejson(Dictionary& dict)
{
	Json::Value& root = GetJson();
	onSavejson(root);
	std::string jsonstr = root.toStyledString(true);
	dict.Add("datastr", jsonstr);
	char szBuffer[4096] = { 0 };
	sprintf_s(szBuffer, 4096, "hmset %s %d %s", sUser, getUserId(), jsonstr.c_str());
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);
	return true;
}

bool Player::onSavejson(Json::Value& root)
{
	if (getSession()) {
		setTotalDownloadBytes(getDownloadBytes() + getSession()->getDownloadBytes());
		setTotalUploadBytes(getUploadBytes() + getSession()->getUploadBytes());
	}

	Json::Value userJson;
	userJson["accId"] = getAccId();
	userJson["userId"] = getUserId();
	userJson["name"] = getName();
	userJson["online"] = GetOnlineTimer();
	userJson["offline"] = GetOfflineTimer();
	userJson["lasthost"] = getHost();
	userJson["onlinetotaltime"] = GetOnlineTime();
	userJson["DownloadBytes"] = (uint32)(getTotalDownloadBytes());
	userJson["UploadBytes"] = (uint32)(getTotalUploadBytes());
	userJson["gold"] = sProperty.getGold(this);
	userJson["diamond"] = sProperty.getDiamond(this);

	Json::Value oldNameJson;
	for (uint32 i = 0; i < mOldNames.size(); ++i) {
		oldNameJson[i] = mOldNames[i];
	}
	userJson["oldNames"] = oldNameJson;

	root["user"] = userJson;
	return true;
}

bool Player::onSaveProperty(Dictionary& dict)
{
	Dictionary aProperyDict;
	aProperyDict.Add(ep_name, getName());
	aProperyDict.Add(ep_CharId, getCharId());
	aProperyDict.Add(ep_mapId, getMapId());
	aProperyDict.Add(ep_speed, getSpeed());
	aProperyDict.Add(ep_posX, getCellX());
	aProperyDict.Add(ep_posY, getCellY());
	aProperyDict.Add(ep_dirPos, getDirPosition());
	aProperyDict.Add(ep_Level, sProperty.getLevel(this));
	aProperyDict.Add(ep_Hp, sProperty.getHp(this));
	aProperyDict.Add(ep_Mp, sProperty.getMp(this));

	BinaryStream bytes;
	bytes << aProperyDict;
	dict.Add("Property", bytes);
	return true;
}

bool Player::onEnterMap()
{
	Map* map = getMap();
	if (!map)
		return false;
	NetEntityEnterMapNotify nfy;
	nfy.name = this->getName();
	nfy.guid = getGuid();
	nfy.mapInsId = getMapInsId();
	nfy.mapId = getMapId();
	sendPacket(nfy);

	float32 cell = map->getMapCell();
	setPos(Utils::Point2ToVector2(getCellPos(), cell, cell));
	return true;
}

bool Player::onLeaveMap()
{
	NetEntityLeaveMapNotify nfy;
	nfy.guid = getGuid();
	nfy.mapInsId = getMapInsId();
	nfy.mapId = getMapId();
	sendPacket(nfy);
	return true;
}

bool Player::onEnterView(Entity* tar)
{
	Entity::onEnterView(tar);
	return tar->onEnterPlayerView(this);
}

bool Player::onLeaveView(Entity* tar)
{
	Entity::onLeaveView(tar);
	return tar->onLeavePlayerView(this);
}

bool Player::onEnterPlayerView(Player* plr)
{
	NetPlayerEnterViewNotify nfy;
	nfy.guid = this->getGuid();
	nfy.name = this->getName();

	nfy.charId = this->getCharId();
	nfy.entityType = this->getEntityType();
	nfy.mapId = this->getMapId();
	nfy.pos.x = this->getCellX();
	nfy.pos.y = this->getCellY();
	nfy.dir = this->getDirPosition();
	nfy.speed = this->getSpeed();
	nfy.status = this->mStatus->getCurrent();
	plr->sendPacket(nfy);
	//LOG_INFO("[%s] onEnterPlayerView [%s]", plr->getName().c_str(), getName().c_str());

	if (!isMoveTo())
		return true;

	NetEntityMoveToNotify moveNfy;
	moveNfy.guid = this->getGuid();
	moveNfy.x = mTarPos.x;
	moveNfy.y = mTarPos.y;
	plr->sendPacket(moveNfy);

	return true;
}

bool Player::onLeavePlayerView(Player* plr)
{
	NetPlayerLeaveViewNotify nfy;
	nfy.guid = this->getGuid();
	nfy.mapId = getMapId();
	plr->sendPacket(nfy);

	//LOG_INFO("[%s] onLeavePlayerView [%s]", plr->getName().c_str(), getName().c_str());
	return true;
}

int32 Player::onTimerSaveDB(TimerEvent& e)
{
	Dictionary dict;
	sApp.doPlayerSave(this, dict);
	return 0;
}

void Player::OnCreate()
{
	LuaEngine::Call(this, sScriptPlayer, "OnCreate", getUserId());
}

void Player::OnDispose()
{
	LuaEngine::Call(this, sScriptPlayer, "OnDispose", getUserId());
}

void Player::OnEnter()
{
	LuaEngine::Call(this, sScriptPlayer, "OnEnter", getUserId());
}

void Player::OnLeave()
{
	LuaEngine::Call(this, sScriptPlayer, "OnLeave", getUserId());
}

void Player::AddOldName(cstring& oldName)
{
	mOldNames.push_back(oldName);
}

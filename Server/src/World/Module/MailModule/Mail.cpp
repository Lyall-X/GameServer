#include "stdafx.h"


bool Mail::operator >> (Json::Value& json)
{
	json["mailType"] = mailType;
	json["fromUserId"] = fromUserId;
	json["targetUserId"] = targetUserId;
	json["title"] = title;
	json["content"] = content;
	json["datastr"] = datastr;
	json["isDown"] = isDown;
	json["isRead"] = isRead;
	return true;
}

bool Mail::operator<<(Json::Value& json)
{
	mailType = (MailType)json["mailType"].asInt();
	fromUserId = json["fromUserId"].asInt();
	targetUserId = json["targetUserId"].asInt();
	title = json["title"].asCString();
	content = json["content"].asCString();
	datastr = json["datastr"].asCString();
	isDown = json["isDown"].asBool();
	isRead = json["isRead"].asBool();
	return true;
}


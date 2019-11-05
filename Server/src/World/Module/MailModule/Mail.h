#pragma once

enum MailType
{
	MailT_None,
	MailT_Target,
	MailT_Total,
};

class MailAttach
{
public:
protected:
private:
};

class Mail
{
public:
	bool operator >> (Json::Value& json);
	bool operator << (Json::Value& json);
public:
	MailType					mailType = MailT_None;
	uint32						fromUserId = 0;
	uint32						targetUserId = 0;
	std::string					title;
	std::string					content;
	std::string					datastr;
	bool						isDown = false; // 是否领取
	bool						isRead = false; // 是否已读
};
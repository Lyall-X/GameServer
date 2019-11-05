#pragma once


class MailModule : public Module
{
	DECLARE_CLASS(MailModule)
public:
	MailModule();
	virtual ~MailModule();
public:
	void SendMail(Mail& aMail);
	void SendMail(cstring& title, cstring& conent);
	void SendMail(MailType mType, uint32 fromUserId, uint32 targetUserId, cstring& title, cstring& conent, cstring& datastr);
	void SendMail(Player* aPlr, MailType mType, cstring& tarName, cstring& title, cstring& conent, cstring& datastr);
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);

	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);
protected:
	int32 onRedisAuth(Event& e);
	int32 onRedisAllPlr(RedisEvent& e);
protected:
	void AddMail(uint32 userId, Mail& aMail);
	void NotifyAddMail(uint32 userId);
	std::vector<Mail>* FindMail(uint32 userId);
	std::string WriteJson(std::vector<Mail>& lstMail);
	void ReadJson(std::vector<Mail>& lstMail, cstring& jsonstr);
	void SendPlrMail(Player* aPlr, std::vector<Mail>& lstMail);
protected:


	std::map<uint32, std::vector<Mail>>			mMapMail;
};

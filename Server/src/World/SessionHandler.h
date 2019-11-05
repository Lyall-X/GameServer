#pragma once
class Session;
class SessionHandler : public Object
{
public:
	SessionHandler();
	~SessionHandler();
protected:
	int onNetSessionEnterNotify(Session* ssn, NetSessionEnterNotify* nfy);
	int onNetSessionLeaveNotify(Session* ssn, NetSessionLeaveNotify* nfy);
	int onNetLoingReq(Session* ssn, NetLoginReq* req);
	int onNetPingNotify(Session* ssn, NetPingNotify* nfy);
private:
};
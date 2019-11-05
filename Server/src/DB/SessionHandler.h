#pragma once

class Session;
class SessionHandler : public Object
{
public:
	SessionHandler();
	~SessionHandler();
protected:
	// sck
	int32 onNetProductListReq(Socket* sck, NetProductListReq* req);
protected:

	int32 onNetSessionEnterNotify(Session* ssn, NetSessionEnterNotify* nfy);
	int32 onNetSessionLeaveNotify(Session* ssn, NetSessionLeaveNotify* nfy);
	int32 onNetLoginReq(Session* ssn, NetLoginReq* req);
protected:
	int32 onNetCreateRoleReq(Player* aPlr, NetCreateRoleReq* req);
	int32 onNetSelectRoleReq(Player* aPlr, NetSelectRoleReq* req);
	int32 onNetQueryRoleReq(Player* aPlr, NetQueryRoleReq* req);
	int32 onNetPlayerSaveNotify(Player* aPlr, NetPlayerSaveNotify* req);
	int32 onNetSellProductReq(Player* aPlr, NetSellProductReq* req);
private:
	int32 DoLogin(NetLoginReq* req, NetLoginRes& res);
	int32 DoQueryRole(NetLoginReq* req, NetLoginRes& res);
	int32 DoCreateRole(NetCreateRoleReq* req, NetCreateRoleRes& res);
	int32 DoSelectRole(NetSelectRoleReq* req, NetSelectRoleRes& res);
};
#pragma once


class Session;
class DBHandler : public Object
{
public:
	DBHandler();
	~DBHandler();
	void doRegister();
protected:
	int32 onNetProductListRes(Socket* sck, NetProductListRes* res);

protected:
	int32 onNetNetLoginRes(Session* ssn, NetLoginRes* res);
protected:
	int32 onNetCreateRoleRes(Player* aPlr, NetCreateRoleRes* res);
	int32 onNetSelectRoleRes(Player* aPlr, NetSelectRoleRes* res);
	int32 onNetQueryRoleRes(Player* aPlr, NetQueryRoleRes* res);
	int32 onNetSellProductRes(Player* aPlr, NetSellProductRes* res);
private:
};
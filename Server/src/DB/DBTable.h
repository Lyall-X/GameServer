#pragma once

#define ACCOUNT_SIZE 32
#pragma pack(1)

class DBAccount :public DBRecord
{
	DECLARE_CLASS(DBAccount);
public:
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);

public:
	std::string user;
	BinaryStream password;
	std::string name;
	std::string email;
	std::string phone;
	uint32 createTime = 0;

	DECLARE_TABLE(DBAccount);
};

class DBUser : public DBRecord
{
	DECLARE_CLASS(DBUser)
public:
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);

public:
	uint32 accountId = 0;
	std::string name;
	uint32 createTime = 0;
	uint32 onlinetotaltime = 0;
	BinaryStream datas;
	std::string datastr;

	DECLARE_TABLE(DBUser);
};

class DBProduct : public DBRecord
{
	DECLARE_CLASS(DBProduct);
public:
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);

public:
	uint32 productId = 0;
	uint32 userId = 0;
	uint32 buyUserId = 0;
	uint32 shelvesTime = 0;
	uint32 unShelvesTime = 0;
	std::string datastr;

	DECLARE_TABLE(DBProduct);
};

#pragma pack()
#include "stdafx.h"


IMPLEMENT_CLASS(DBAccount, DBRecord);

IMPLEMENT_TABLE_BEGIN(DBAccount, "account")

IMPLEMENT_RECORD_INFO(DBAccount, id, Variant::TypeInt32, "NULL", KeyTypeAutoInc)
IMPLEMENT_RECORD_INFO(DBAccount, user, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, password, Variant::TypeMemory, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, name, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, email, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, phone, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, createTime, Variant::TypeInt32, "NULL", KeyTypeNone)

IMPLEMENT_TABLE_END(DBAccount, "account")

bool DBAccount::operator >> (BinaryStream& bytes)
{
	CHECK(__super::operator >>(bytes));
	CHECK(bytes << user);
	CHECK(bytes << password);
	CHECK(bytes << name);
	CHECK(bytes << email);
	CHECK(bytes << phone);
	CHECK(bytes << createTime);
	return true;
}

bool DBAccount::operator<<(BinaryStream& bytes)
{
	CHECK(__super::operator >>(bytes));
	CHECK(bytes >> user);
	CHECK(bytes >> password);
	CHECK(bytes >> name);
	CHECK(bytes >> email);
	CHECK(bytes >> phone);
	CHECK(bytes >> createTime);
	return true;
}

IMPLEMENT_CLASS(DBUser, DBRecord);
IMPLEMENT_TABLE_BEGIN(DBUser, "user")

IMPLEMENT_RECORD_INFO(DBUser, id, Variant::TypeInt32, "NULL", KeyTypeAutoInc)
IMPLEMENT_RECORD_INFO(DBUser, accountId, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, name, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, createTime, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, onlinetotaltime, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, datas, Variant::TypeMemory, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, datastr, Variant::TypeString, "NULL", KeyTypeNone)

IMPLEMENT_TABLE_END(DBUser, "user")

bool DBUser::operator >> (BinaryStream& bytes)
{
	CHECK(__super::operator >>(bytes));
	CHECK(bytes << accountId);
	CHECK(bytes << name);
	CHECK(bytes << createTime);
	CHECK(bytes << onlinetotaltime);
	CHECK(bytes << datas);
	CHECK(bytes << datastr);
	return true;
}

bool DBUser::operator<<(BinaryStream& bytes)
{
	CHECK(__super::operator <<(bytes));
	CHECK(bytes >> accountId);
	CHECK(bytes >> name);
	CHECK(bytes >> createTime);
	CHECK(bytes >> onlinetotaltime);
	CHECK(bytes >> datas);
	CHECK(bytes >> datastr);
	return true;
}


IMPLEMENT_CLASS(DBProduct, DBRecord);

IMPLEMENT_TABLE_BEGIN(DBProduct, "product")

IMPLEMENT_RECORD_INFO(DBProduct, id, Variant::TypeInt32, "NULL", KeyTypeAutoInc)
IMPLEMENT_RECORD_INFO(DBProduct, productId, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBProduct, userId, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBProduct, buyUserId, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBProduct, shelvesTime, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBProduct, unShelvesTime, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBProduct, datastr, Variant::TypeString, "NULL", KeyTypeNone)

IMPLEMENT_TABLE_END(DBProduct, "product")

bool DBProduct::operator >> (BinaryStream& bytes)
{
	CHECK(__super::operator >> (bytes));
	CHECK(bytes << productId);
	CHECK(bytes << userId);
	CHECK(bytes << buyUserId);
	CHECK(bytes << shelvesTime);
	CHECK(bytes << unShelvesTime);
	CHECK(bytes << datastr);
	return true;
}

bool DBProduct::operator<<(BinaryStream& bytes)
{
	CHECK(__super::operator <<(bytes));
	CHECK(bytes >> productId);
	CHECK(bytes >> userId);
	CHECK(bytes >> buyUserId);
	CHECK(bytes >> shelvesTime);
	CHECK(bytes >> unShelvesTime);
	CHECK(bytes >> datastr);
	return true;
}

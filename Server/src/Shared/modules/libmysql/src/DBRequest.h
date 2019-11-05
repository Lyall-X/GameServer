#pragma once

typedef std::vector<DBRecord*> DBRecordPtrList;

cstring ExecuteQuery(void* mysql, cstring& tablename, uint32& result_count);
cstring ExecuteQuery(void* mysql, cstring& tablename, DBQueryResult* query_result);
cstring ExecuteQuery(void* mysql, cstring& sqlstr, DBRecordPtrList& result_records, uint32& result_count, uint32 result_max_count, DBQueryResult* query_result);
cstring ExecuteQuery(void* mysql, DBRecord* query_record, DBRecordPtrList& result_records, uint32& result_count, cstring& comparestr, cstring& returnstr, int32 result_max_count);

cstring ExecuteInsert(void* mysql, DBRecord* insert_record, cstring& comparestr, cstring& exclutionstr);
cstring ExecuteInsert(void* mysql, cstring& cmdstr);

cstring ExecuteUpdate(void* mysql, DBRecord* update_record, cstring& comparestr, uint32& update_rows, cstring& updatestr);
cstring ExecuteUpdate(void* mysql, cstring& cmdstr, uint32& update_rows);

cstring ExecuteDelete(void* mysql, DBRecord* deleteRecord, cstring& comparestr, uint32& delete_rows);
cstring ExecuteDelete(void* mysql, cstring& cmdstr, uint32& delete_rows);

class DBRequest : public EventDispatcher
{
	friend class DBAngent;
public:
	DBRequest(DBRecord* dbRecord);
	virtual ~DBRequest();
	void* mysql = 0;
	DBRecord* record = 0;
	std::string cmd;
	std::string compareRecord;		// 操作哪些字段 没有就是全部
	std::string exclutionRecord;	// 检测是否已经存在
	std::string err;
protected:
	virtual bool Execute() = 0;
protected:
private:
};


class DBQueryRequest : public DBRequest
{
public:
	DBQueryRequest(DBRecord* dbRecord);
	~DBQueryRequest();
	std::vector<DBRecord*> records;
	uint32 maxCount = 0;
	uint32 resultCount = 0;
	std::string resultRecord;		// 返回哪些字段 没有就是全部
	DBQueryResult* result = NULL;
protected:
	bool Execute();
	std::string Query();
protected:
private:
};

class DBInsertRequest : public DBRequest
{
public:
	DBInsertRequest(DBRecord* dbRecord);
	std::string resultRecord;
protected:
	bool Execute();
protected:
private:
};

class DBUpdateRequest : public DBRequest
{
public:
	DBUpdateRequest(DBRecord* dbRecord);
	uint32	updateRows = 0;
	std::string updateRecord;		// 更新哪些字段 没有就是全部
protected:
	bool Execute();
protected:
private:
};

class DBDeleteRequest : public DBRequest
{
public:
	DBDeleteRequest(DBRecord* dbRecord);
	uint32 deleteRows;
protected:
	bool Execute();
protected:
private:
};

#pragma once

class DBRecord;
class QueryResult;
class InsertResult;
class UpdateResult;
class DeleteResult;


class DBConnector : public Object
{
public:
	DBConnector();
	~DBConnector();
	bool connect(const std::string& host, const std::string& user, const std::string& password, const std::string& name, int16 port);
	void update(float time, float delay);
	void Destroy();
public: //异步;
	int8* Query(const DBRecord& query_record_, DBRecord* result_records, uint32* result_count_, const uint32 result_max_count = 0);
	int8* Query(const DBRecord& query_record_, DBRecord* result_records_, uint32* result_count_, const int8* compare_record_names_, const int8* return_record_names_, int32 result_max_count = 0);
	int8* Query(const DBRecord& query_record_, const int8* sql_name_, DBRecord* result_records_, uint32* result_count_, uint32 result_max_count = 0);
	int8* Insert(const DBRecord& insert_record_, const int8* compare_record_names_ = 0, const int8* exclution_record_names_ = 0);
	int8* Update(const DBRecord& update_record_, const int8* compare_record_names_, uint32* update_rows_, const int8* update_record_names_);
	int8* Update(const DBRecord& update_record_, const int8* sql_name_, uint32* update_rows_);
	int8* Delete(const DBRecord& delete_record_, const int8* compare_record_names_, uint32* update_rows_);
private:
	void process();
	virtual uint32 ThreadProcess(Threader& threader);
	int32 threaderRun(Threader& theader);
public: 
	// 同步;
	int8* doQuery(const DBRecord& query_record, std::vector<DBRecord*>& result_records, uint32& result_count, const uint32 result_max_count = 0);
	int8* doQuery(const DBRecord& query_record, std::vector<DBRecord*>& result_records, uint32& result_count, const std::string& compare_record_names, const std::string& return_record_names, int32 result_max_count = 0);
	// 同步;
	int8* doQuery(const DBRecord& query_record, DBRecord& result_record, uint32& result_count, const uint32 result_max_count = 0);
	int8* doQuery(const DBRecord& query_record, DBRecord& result_record, uint32& result_count, const std::string& compare_record_names, const std::string& return_record_names, int32 result_max_count = 0);

	int8* doQuery(std::vector<DBRecord*>& result_records, const std::string& sqlstr, uint32& result_count, uint32 result_max_count = 0);
	int8* doQuery(const std::string& tablename, uint32& result_count);

	int8* doQuery(const std::string& tablename, DBQueryResult* query_result);

	int8* doInsert(const DBRecord& insert_record, const std::string& compare_record_names = "", const std::string& exclution_record_names = "");
	int8* doUpdate(const DBRecord& update_record, const std::string& compare_record_names, uint32& update_rows, const std::string& update_record_names);
	int8* doUpdate(const DBRecord& update_record, const std::string& sql_name, uint32& update_rows);
	int8* doDelete(const DBRecord& delete_record, const std::string& compare_record_names, uint32& update_rows);
protected:
	int8* doQuery(const std::string& sqlstr, std::vector<DBRecord*>& result_records, uint32& result_count, uint32 result_max_count = 0, DBQueryResult* query_result = NULL);
protected:
	struct st_mysql*		mMysql;
private:
	std::string				mHost;
	std::string				mUser;
	std::string				mPassword;
	std::string				mName;
	int16					mPort;

protected:
	std::queue<QueryResult*> taskQueryQueue;
	std::queue<InsertResult*> taskInsertQueue;
	std::queue<UpdateResult*> taskUpdateQueue;
	std::queue<DeleteResult*> taskDeleteQueue;

	std::queue<QueryResult*> completeQueryQueue;
	std::queue<InsertResult*> completeInsertQueue;
	std::queue<UpdateResult*> completeUpdateQueue;
	std::queue<DeleteResult*> completeDeleteQueue;
	Mutex					_mutex;
	Threader*				mThreader;
};
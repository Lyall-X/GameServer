#include "mysql.hpp"
#include <mysql.h>

DBConnector::DBConnector():
mMysql(NULL),
mThreader(NULL)
{
}

DBConnector::~DBConnector()
{

}

void DBConnector::Destroy()
{
	if (mMysql) {
		mysql_close(mMysql);
		delete mMysql;
		mMysql = NULL;
	}

	if (mThreader) {
		mThreader->Eixt();
	}
	SAFE_DELETE(mThreader);
}

bool DBConnector::connect(const std::string& host, const std::string& user, const std::string& password, const std::string& name, int16 port)
{
	SAFE_DELETE(mMysql);
	mMysql = new MYSQL;

	mHost = host;
	mUser = user;
	mPassword = password;
	mName = name;
	mPort = port;

	mMysql = mysql_init(mMysql);

	mysql_options(mMysql, MYSQL_SET_CHARSET_NAME, "gb2312");
	my_bool reconnect = true;
	mysql_options(mMysql, MYSQL_OPT_RECONNECT, &reconnect);
	if (!mysql_real_connect(mMysql, mHost.c_str(), mUser.c_str(), mPassword.c_str(), mName.c_str(), mPort, NULL, 0))
		return false;

	
	//mysql_query(sql_account, "set character set'gbk'");
	//mysql_query(sql_account, "set names'gbk'");
	//mysql_set_character_set(sql_account, "set character set'gbk'");

	//if (!mysql_set_character_set(sql_account, "utf8"))
	//{
	//	LOG_DEBUG(LogSystem::csl_color_green, "<<<New client character set: [%s] ...", mysql_character_set_name(sql_account));
	//}

	//_threader = Threader::createThread(this, (Threader::ThreadCallBack)(&DBConnector::threaderRun));

	return true;
}

int8* DBConnector::Query(const DBRecord& query_record_, DBRecord* result_records_, uint32* result_count_, const int8* compare_record_names_, const int8* return_record_names_, int32 result_max_count /* = 0 */)
{
	//return doQuery((DBRecord&)query_record_, result_records_, result_count_, compare_record_names_, return_record_names_, result_max_count);
	QueryResult* result = new QueryResult;
	result->record = (DBRecord*)&query_record_;
	result->records = result_records_;
	result->queryCount = result_count_;
	result->resultCount = result_max_count;
	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->resultRecord = return_record_names_ == NULL ? "" : return_record_names_;

	_mutex.lock();
	taskQueryQueue.push(result);		
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Query(const DBRecord& query_record_, DBRecord* result_records, uint32* result_count_, const uint32 result_max_count /* = 0 */)
{
	//return doQuery((DBRecord&)query_record_, result_records, result_count_, result_max_count);
	QueryResult* result = new QueryResult;
	result->record = (DBRecord*)&query_record_;
	result->records = result_records;
	result->queryCount = result_count_;
	result->resultCount = result_max_count;
	_mutex.lock();
	taskQueryQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Query(const DBRecord& query_record_, const int8* sql_name_, DBRecord* result_records_, uint32* result_count_, uint32 result_max_count /*= 0*/)
{
	//return doQuery((DBRecord&)query_record_, sql_name_, result_records_, result_count_, result_max_count);
	QueryResult* result = new QueryResult;
	result->record = (DBRecord*)&query_record_;
	result->records = result_records_;
	result->queryCount = result_count_;
	result->resultCount = result_max_count;
	result->cmd = sql_name_;
	_mutex.lock();
	taskQueryQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Insert(const DBRecord& insert_record_, const int8* compare_record_names_ /* = 0 */, const int8* exclution_record_names_ /* = 0 */)
{
	//return doInsert((DBRecord&)insert_record_, compare_record_names_);
	InsertResult* result = new InsertResult;
	result->record = (DBRecord*)&insert_record_;
	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->exclutionRecord = exclution_record_names_ == NULL ? "" : exclution_record_names_;
	_mutex.lock();
	taskInsertQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Update(const DBRecord& update_record_, const int8* compare_record_names_, uint32* update_rows_, const int8* update_record_names_)
{
	//return doUpdate((DBRecord&)update_record_, compare_record_names_, update_rows_, update_record_names_);
	UpdateResult* result = new UpdateResult;
	result->record = (DBRecord*)&update_record_;
	result->updateRows = update_rows_;

	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->updateRecord = update_record_names_ == NULL ? "" : update_record_names_;
	_mutex.lock();
	taskUpdateQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Update(const DBRecord& update_record_, const int8* sql_name_, uint32* update_rows_)
{
	UpdateResult* result = new UpdateResult;
	result->record = (DBRecord*)&update_record_;
	result->updateRows = update_rows_;
	result->cmd = sql_name_;
	_mutex.lock();
	taskUpdateQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Delete(const DBRecord& delete_record_, const int8* compare_record_names_, uint32* update_rows_)
{
	DeleteResult* result = new DeleteResult;
	result->record = (DBRecord*)&delete_record_;

	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->deleteRows = update_rows_;
	_mutex.lock();
	taskDeleteQueue.push(result);
	_mutex.unlock();
	return 0;
}

void DBConnector::process()
{
}

int8* DBConnector::doQuery(const DBRecord& query_record, std::vector<DBRecord*>& result_records, uint32& result_count, const uint32 result_max_count /*= 0*/)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 sql_size_ = 0;
	if (!GetQuerySqlCmd(sql_cmd_, sql_size_, (DBRecord*)&query_record, result_max_count, NULL, NULL))
	{
		return "";
	}
	return 0;
}

int8* DBConnector::doQuery(const DBRecord& query_record, std::vector<DBRecord*>& result_records, uint32& result_count, const std::string& compare_record_names, const std::string& return_record_names, int32 result_max_count /* = 0 */)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 sql_size_ = 0;
	GetQuerySqlCmd(sql_cmd_, sql_size_, (DBRecord*)&query_record, result_max_count, compare_record_names.c_str(), return_record_names.c_str());
	return doQuery(sql_cmd_, result_records, result_count, result_max_count);
}

int8* DBConnector::doQuery(std::vector<DBRecord *>& result_records, const std::string& sqlstr, uint32& result_count, uint32 result_max_count /* = 0 */)
{
	return doQuery(sqlstr, result_records, result_count, result_max_count);
}

int8* DBConnector::doQuery(const DBRecord& query_record, DBRecord& result_record, uint32& result_count, const uint32 result_max_count /*= 0*/)
{
	std::vector<DBRecord*> result_records;
	result_records.push_back(&result_record);
	return doQuery(query_record, result_records, result_count, result_max_count);
}

int8* DBConnector::doQuery(const DBRecord& query_record, DBRecord& result_record, uint32& result_count, const std::string& compare_record_names, const std::string& return_record_names, int32 result_max_count /*= 0*/)
{
	std::vector<DBRecord*> result_records;
	result_records.push_back(&result_record);
	return doQuery(query_record, result_records, result_count, compare_record_names, return_record_names, result_max_count);
}

int8* DBConnector::doQuery(const std::string& tablename, uint32& result_count)
{
	char sqlstr[256] = {};
	sprintf_s(sqlstr, 256, "select count(*) from %s", tablename.c_str());
	if (int iError = mysql_real_query(mMysql, sqlstr, (unsigned long)strlen(sqlstr))) {
		return (int8*)mysql_error(mMysql);
	}
	MYSQL_RES*  result_ = mysql_store_result(mMysql);
	if (result_ == NULL) {
		const int8* err = mysql_error(mMysql);
		if (!err || strlen(err) == 0)
			return NULL;
		return (int8*)err;
	}
	result_count = (int32)mysql_num_rows(result_);
	mysql_free_result(result_);
	return 0;
}

int8* DBConnector::doQuery(const std::string& tablename, DBQueryResult* query_result)
{
	char sqlstr[256] = {};
	sprintf_s(sqlstr, 256, "select * from %s", tablename.c_str());
	uint32 result_count = 0;
	uint32 result_max_count = 0;
	std::vector<DBRecord *> result_records;
	return doQuery(sqlstr, result_records, result_count, result_max_count, query_result);
}

int8* DBConnector::doQuery(const std::string& sqlstr, std::vector<DBRecord *>& result_records, uint32& result_count, uint32 result_max_count /* = 0 */, DBQueryResult* query_result /* = NULL */)
{
	int32 iError = mysql_real_query(mMysql, sqlstr.c_str(), (unsigned long)sqlstr.length());
	if (iError){
		return (int8*)mysql_error(mMysql);
	}

	MYSQL_RES*  result_ = mysql_store_result(mMysql);
	if (result_ == NULL) {
		const int8* err = mysql_error(mMysql);
		if (!err || strlen(err) == 0)
			return NULL;
		return (int8*)err;
	}

	uint32 num_rows_ = (int32)mysql_num_rows(result_);
	if (result_max_count && num_rows_ > result_max_count)
		num_rows_ = result_max_count;
	
	uint32 num_fields = mysql_num_fields(result_);
	std::vector<std::string> fields;
	MYSQL_FIELD* field = mysql_fetch_field(result_);
	while (field)
	{
		fields.push_back(field->name);
		field = mysql_fetch_field(result_);
	}

	if (result_records.size() > 0 && query_result == NULL) {

		TableDescriptor& descriptor_ = result_records[0]->getThisDescriptor();
		MYSQL_ROW row_ = 0;
		for (int32 idxRow = 0; idxRow < num_rows_; ++idxRow) {

			row_ = mysql_fetch_row(result_);
			if (!row_) return "fetch result failed";
			unsigned long* lengths = mysql_fetch_lengths(result_);
			for (int32 idxCol = 0; idxCol < fields.size(); idxCol++)
			{
				FieldDescriptor* record = descriptor_.getFieldDescriptor(fields[idxCol]);
				if (record == NULL) continue;
				GetValueRecord(result_records[idxRow], (const FieldDescriptor&)*record, row_[idxCol], lengths[idxCol]);
			}
		}
	}
	else
	{
		query_result->fields = fields;
		query_result->length = num_rows_;
		query_result->rows = new DBRowResult[query_result->length];

		MYSQL_ROW row_ = 0;
		for (int32 idxRow = 0; idxRow < num_rows_; ++idxRow) {

			row_ = mysql_fetch_row(result_);
			if (!row_) return "fetch result failed";
			DBRowResult& dbRowResult = query_result->rows[idxRow];
			dbRowResult.fieldCount = num_fields;
			dbRowResult.fields = new DBField[dbRowResult.fieldCount];

			unsigned long* lengths = mysql_fetch_lengths(result_);
			for (int32 idxCol = 0; idxCol < fields.size(); idxCol++)
			{
				DBField& dbField = dbRowResult.fields[idxCol];
				dbField.length = lengths[idxCol];
				if (dbField.length <= 0) continue;
			
				dbField.dataptr = new char[dbField.length];
				memcpy(dbField.dataptr, row_[idxCol], dbField.length);
			}
		}
	}

	result_count = num_rows_;
	mysql_free_result(result_);
	return 0;
}

int8* DBConnector::doInsert(const DBRecord& insert_record, const std::string& compare_record_names /* = "" */, const std::string& exclution_record_names /* = "" */)
{
	if (exclution_record_names.length() > 0)
	{
		uint32 count = 0;
		std::vector<DBRecord*> result_records;
		result_records.push_back((DBRecord*)&insert_record);
		doQuery(insert_record, result_records, count, exclution_record_names, "");
		if (count > 0) return "ÒÑ´æÔÚ..";
	}

	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;

	if (!GetInsertSqlCmd(sql_cmd_, size_, (DBRecord*)&insert_record, compare_record_names.c_str()))
		return "error get insert sql cmd .";

	if (int err = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
		return (int8*)mysql_error(mMysql);

	DBRecord& record = (DBRecord&)insert_record;
	record.id = (int32)mysql_insert_id(mMysql);

	return 0;
}

int8* DBConnector::doUpdate(const DBRecord& update_record, const std::string& compare_record_names, uint32& update_rows, const std::string& update_record_names)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	int32 size_ = 0;

	if (!GetUpdateSqlCmd(sql_cmd_, size_, (DBRecord*)&update_record, compare_record_names.c_str(), update_record_names.c_str()))
		return "";

	if (int iError = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
		return (int8*)mysql_error(mMysql);

	update_rows = (int)mysql_affected_rows(mMysql);

	return 0;
}

int8* DBConnector::doUpdate(const DBRecord& update_record, const std::string& sql_name, uint32& update_rows)
{
	return 0;
}

int8* DBConnector::doDelete(const DBRecord& delete_record, const std::string& compare_record_names, uint32& update_rows)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;

	if (!GetDeleteSqlCmd(sql_cmd_, size_, (DBRecord*)&delete_record, compare_record_names))
		return "";

	if (int iError = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
		return (int8*)mysql_error(mMysql);

	update_rows = (unsigned long)mysql_affected_rows(mMysql);

	return 0;
}

void DBConnector::update(float time, float delay)
{
	//if (mysql_ping(mMysql)) {
	//	connect(mHost, mUser, mPassword, mName, mPort);
	//}

	//process();
	if (completeQueryQueue.size() > 0)
	{
		_mutex.lock();
		QueryResult * result = completeQueryQueue.front();
		completeQueryQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::QUERY;
		dbEvent.result = result;
		result->record->Dispatch(dbEvent);
		
		delete result;
	}

	if (completeInsertQueue.size() > 0)
	{
		_mutex.lock();
		InsertResult * result = completeInsertQueue.front();
		completeInsertQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::INSERT;
		dbEvent.result =result;
		result->record->Dispatch(dbEvent);
		delete result;
	}

	if (completeUpdateQueue.size() > 0)
	{
		_mutex.lock();
		UpdateResult * result = completeUpdateQueue.front();
		completeUpdateQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::UPDATE;
		dbEvent.result = result;
		result->record->Dispatch(dbEvent);
		delete result;
	}

	if (completeDeleteQueue.size() > 0)
	{
		_mutex.lock();
		DeleteResult * result = completeDeleteQueue.front();
		completeDeleteQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::REMOVE;
		dbEvent.result = result;
		result->record->Dispatch(dbEvent);

		delete result;
	}
}

int32 DBConnector::threaderRun(Threader& theader)
{
	((DBConnector*)theader.m_thisObject)->process();
	Threader::sleep(1);
	return 0;
}

uint32 DBConnector::ThreadProcess(Threader& threader)
{
	while (threader.Active())
	{
		process();
		Threader::sleep(1);
	}
	return 0;
}


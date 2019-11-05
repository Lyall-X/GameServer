#include "mysql.hpp"
#include <mysql.h>

cstring ExecuteQuery(void* mysql, cstring& tablename, DBQueryResult* query_result)
{
	char sqlstr[256] = {};
	sprintf_s(sqlstr, 256, "select * from %s", tablename.c_str());
	uint32 result_count = 0;
	uint32 result_max_count = 0;
	std::vector<DBRecord *> result_records;
	return ExecuteQuery(mysql, sqlstr, result_records, result_count, result_max_count, query_result);
}

cstring ExecuteQuery(void* mysql, cstring& sqlstr, DBRecordPtrList& result_records, uint32& result_count, uint32 result_max_count, DBQueryResult* query_result)
{
	MYSQL* _mysql = (MYSQL*)mysql;

	int32 iError = mysql_real_query(_mysql, sqlstr.c_str(), (unsigned long)sqlstr.length());
	if (iError) {
		return (int8*)mysql_error(_mysql);
	}

	MYSQL_RES*  result_ = mysql_store_result(_mysql);
	if (result_ == NULL) {
		const int8* err = mysql_error(_mysql);
		if (!err || strlen(err) == 0)
			return "";
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
		if (query_result == NULL) return "";
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
	return "";
}

cstring ExecuteQuery(void* mysql, cstring& tablename, uint32& result_count)
{
	MYSQL* _mysql = (MYSQL*)mysql;

	char sqlstr[256] = {};
	sprintf_s(sqlstr, 256, "select count(*) from %s", tablename.c_str());
	if (int iError = mysql_real_query(_mysql, sqlstr, (unsigned long)strlen(sqlstr))) {
		return (int8*)mysql_error(_mysql);
	}
	MYSQL_RES*  result_ = mysql_store_result(_mysql);
	if (result_ == NULL) {
		const int8* err = mysql_error(_mysql);
		if (!err || strlen(err) == 0)
			return NULL;
		return (int8*)err;
	}
	result_count = (int32)mysql_num_rows(result_);
	mysql_free_result(result_);
	return "";
}


cstring ExecuteQuery(void* mysql, DBRecord* query_record, DBRecordPtrList& result_records, uint32& result_count, cstring& comparestr, cstring& returnstr, int32 result_max_count)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 sql_size_ = 0;
	GetQuerySqlCmd(sql_cmd_, sql_size_, query_record, result_max_count, comparestr.c_str(), returnstr.c_str());
	return ExecuteQuery(mysql, sql_cmd_, result_records, result_count, result_max_count, NULL);
}

cstring ExecuteInsert(void* mysql, DBRecord* insert_record, cstring& comparestr, cstring& exclutionstr)
{
	MYSQL* _mysql = (MYSQL*)mysql;

	if (exclutionstr.length() > 0)
	{
		uint32 count = 0;
		std::vector<DBRecord*> result_records;
		result_records.push_back((DBRecord*)&insert_record);
		ExecuteQuery(mysql, insert_record, result_records, count, exclutionstr, "", 0);
		if (count > 0) return "已存在..";
	}

	char cmdstr[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;

	if (!GetInsertSqlCmd(cmdstr, size_, insert_record, comparestr.c_str()))
		return "error get insert sql cmd .";

	if (int err = mysql_real_query(_mysql, cmdstr, (unsigned long)strlen(cmdstr)))
		return mysql_error(_mysql);

	//DBRecord& record = (DBRecord&)insert_record;
	//record.id = (int32)mysql_insert_id(mMysql);
	return "";
}


cstring ExecuteInsert(void* mysql, cstring& cmdstr)
{
	MYSQL* _mysql = (MYSQL*)mysql;
	if (int err = mysql_real_query(_mysql, cmdstr.c_str(), (unsigned long)cmdstr.length()))
		return mysql_error(_mysql);

	//DBRecord& record = (DBRecord&)insert_record;
	//record.id = (int32)mysql_insert_id(mMysql);
	return "";
}

cstring ExecuteUpdate(void* mysql, DBRecord* update_record, cstring& comparestr, uint32& update_rows, cstring& updatestr)
{
	MYSQL* _mysql = (MYSQL*)mysql;
	char cmdstr[SQL_CMD_COUNT] = { 0 };
	int32 size_ = 0;

	if (!GetUpdateSqlCmd(cmdstr, size_, update_record, comparestr.c_str(), updatestr.c_str()))
		return "";

	if (int iError = mysql_real_query(_mysql, cmdstr, (unsigned long)strlen(cmdstr)))
		return mysql_error(_mysql);

	update_rows = (int)mysql_affected_rows(_mysql);
	return "";
}


cstring ExecuteUpdate(void* mysql, cstring& cmdstr, uint32& update_rows)
{
	MYSQL* _mysql = (MYSQL*)mysql;
	if (int iError = mysql_real_query(_mysql, cmdstr.c_str(), (unsigned long)cmdstr.length()))
		return mysql_error(_mysql);
	update_rows = (uint32)mysql_affected_rows(_mysql);
	return "";
}


cstring ExecuteDelete(void* mysql, DBRecord* deleteRecord, cstring& comparestr, uint32& delete_rows)
{
	MYSQL* _mysql = (MYSQL*)mysql;
	char cmdstr[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;

	if (!GetDeleteSqlCmd(cmdstr, size_, deleteRecord, comparestr))
		return "";

	if (int iError = mysql_real_query(_mysql, cmdstr, (unsigned long)strlen(cmdstr)))
		return mysql_error(_mysql);
	delete_rows = (unsigned long)mysql_affected_rows(_mysql);
	return "";
}


cstring ExecuteDelete(void* mysql, cstring& cmdstr, uint32& delete_rows)
{
	MYSQL* _mysql = (MYSQL*)mysql;
	if (int iError = mysql_real_query(_mysql, cmdstr.c_str(), (unsigned long)cmdstr.length()))
		return mysql_error(_mysql);
	delete_rows = (unsigned long)mysql_affected_rows(_mysql);
	return "";
}


DBQueryRequest::DBQueryRequest(DBRecord* dbRecord):
DBRequest(dbRecord)
{

}

DBQueryRequest::~DBQueryRequest()
{
	for (DBRecord* rcd : records) {
		delete rcd;
	}
	records.clear();
	releaseResult(result);
	result = NULL;
}

bool DBQueryRequest::Execute()
{
	char cmdstr[SQL_CMD_COUNT] = { 0 };
	uint32 sql_size_ = 0;
	if (compareRecord == "") {
		sprintf_s(cmdstr, 256, "select * from %s", record->getThisDescriptor().tableName);
	}
	else {
		GetQuerySqlCmd(cmdstr, sql_size_, record, maxCount, compareRecord.c_str(), resultRecord.c_str());
	}
	cmd = cmdstr;
	err = Query();
	if (err != "")
		return false;
	return true;
}


std::string DBQueryRequest::Query()
{
	MYSQL* _mysql = (MYSQL*)mysql;
	int32 iError = mysql_real_query(_mysql, cmd.c_str(), (unsigned long)cmd.length());
	if (iError) {
		return (int8*)mysql_error(_mysql);
	}

	MYSQL_RES*  result_ = mysql_store_result(_mysql);
	if (result_ == NULL) {
		return mysql_error(_mysql);
	}

	uint32 num_rows_ = (int32)mysql_num_rows(result_);
	if (maxCount && num_rows_ > maxCount)
		num_rows_ = maxCount;

	uint32 num_fields = mysql_num_fields(result_);
	std::vector<std::string> fields;
	MYSQL_FIELD* field = mysql_fetch_field(result_);
	while (field)
	{
		fields.push_back(field->name);
		field = mysql_fetch_field(result_);
	}

	for (uint32 i = 0; i < num_rows_; ++i) {
		records.push_back((DBRecord*)record->createInstance());
	}

	for (int32 idxRow = 0; idxRow < num_rows_; ++idxRow) {

		TableDescriptor& descriptor_ = records[idxRow]->getThisDescriptor();
		MYSQL_ROW row_ = 0;
		row_ = mysql_fetch_row(result_);
		if (!row_) return "fetch result failed";
		unsigned long* lengths = mysql_fetch_lengths(result_);
		for (int32 idxCol = 0; idxCol < fields.size(); idxCol++)
		{
			FieldDescriptor* record = descriptor_.getFieldDescriptor(fields[idxCol]);
			if (record == NULL) continue;
			GetValueRecord(records[idxRow], (const FieldDescriptor&)*record, row_[idxCol], lengths[idxCol]);
		}
	}

	resultCount = num_rows_;
	mysql_free_result(result_);
	return "";
}


DBInsertRequest::DBInsertRequest(DBRecord* dbRecord):
DBRequest(dbRecord)
{

}

bool DBInsertRequest::Execute()
{
	if (exclutionRecord != "") {
		uint32 count = 0;
		std::vector<DBRecord*> result_records;
		result_records.push_back((DBRecord*)&record);
		ExecuteQuery(mysql, record, result_records, count, exclutionRecord, "", 0);
		if (count > 0) {
			err = "已存在..";
			return false;
		}
	}

	char cmdstr[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;
	GetInsertSqlCmd(cmdstr, size_, record, compareRecord);
	cmd = cmdstr;
	err = ExecuteInsert(mysql, record, compareRecord, exclutionRecord);
	if (err != "")
		return false;
	return true;
}


DBUpdateRequest::DBUpdateRequest(DBRecord* dbRecord):
DBRequest(dbRecord)
{

}

bool DBUpdateRequest::Execute()
{
	char cmdstr[SQL_CMD_COUNT] = { 0 };
	int32 size_ = 0;
	GetUpdateSqlCmd(cmdstr, size_, record, compareRecord, updateRecord);
	cmd = cmdstr;
	err = ExecuteUpdate(mysql, cmd, updateRows);
	if (err != "")
		return false;
	return true;
}


DBDeleteRequest::DBDeleteRequest(DBRecord* dbRecord):
DBRequest(dbRecord)
{

}

bool DBDeleteRequest::Execute()
{
	char cmdstr[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;

	GetDeleteSqlCmd(cmdstr, size_, record, compareRecord);
	cmd = cmdstr;
	err = ExecuteDelete(mysql, cmd, deleteRows);
	if (err != "")
		return false;
	return true;
}


DBRequest::DBRequest(DBRecord* dbRecord):
record(dbRecord)
{

}

DBRequest::~DBRequest()
{
	SAFE_DELETE(record);
}

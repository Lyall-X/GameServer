#pragma once

// 数据字段类型;
enum KeyType
{
	KeyTypeNone,
	KeyTypeAutoInc,		// 自增长;
};

struct FieldDescriptor
{
	const char*				field;
	const char*				default;
	uint32					offset;
	uint8					typevalue;
	uint8					typekey;
};

struct TableDescriptor
{
	FieldDescriptor*			records;
	uint32						recordCount;
	const char*					tableName;
	FieldDescriptor* getFieldDescriptor(const std::string& field) const;
};

class DBRecord : public EventDispatcher
{
	DECLARE_CLASS(DBRecord);
public:
	static TableDescriptor& getDescriptor() { static TableDescriptor desc; return desc; }
	virtual TableDescriptor& getThisDescriptor()  { return DBRecord::getDescriptor(); }
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);
public:
	uint32 id = 0;
};

struct DBField
{
	char* dataptr = NULL;
	uint32 length = 0;
};

struct DBRowResult
{
	uint32 fieldCount = 0;
	DBField* fields = NULL;
};

struct DBQueryResult
{
	std::vector<std::string> fields;
	uint32 length = 0;
	DBRowResult* rows = NULL;
};

template<class T>
T* swapQueryResult(DBQueryResult* result)
{
	if (result->length <= 0) return NULL;

	T* dest = new T[result->length];
	TableDescriptor& descriptor_ = T::getDescriptor();
	for (uint32 i = 0; i < result->length; ++i)
	{
		DBRowResult& rowResult = result->rows[i];
		for (uint32 j = 0; j < rowResult.fieldCount; ++j)
		{
			FieldDescriptor* record = descriptor_.getFieldDescriptor(result->fields[j]);
			if (record == NULL) continue;
			DBField& field = rowResult.fields[j];
			GetValueRecord(&dest[i], (const FieldDescriptor&)*record, field.dataptr, field.length);
		}
	}
	return dest;
}

void releaseResult(DBQueryResult* result);

class DBResult
{
public:
	DBRecord* record;
	std::string cmd;
	std::string compareRecord;
	std::string exclutionRecord;
	std::string err;
	DBResult();
	virtual ~DBResult();
};

class QueryResult : public DBResult
{
public:
	DBRecord* records;
	uint32* queryCount;
	uint32 resultCount;
	std::string resultRecord;
	QueryResult();
	virtual ~QueryResult();
};

class InsertResult : public DBResult
{
public:
	std::string resultRecord;
	InsertResult();
	virtual ~InsertResult();
};

class UpdateResult : public DBResult
{
public:
	uint32* updateRows;
	std::string updateRecord;
	UpdateResult();
	virtual ~UpdateResult();
};


class DeleteResult : public DBResult
{
public:
	uint32* deleteRows;
	DeleteResult();
	virtual ~DeleteResult();
};

#define DECLARE_TABLE(class_name)\
public:\
	TableDescriptor& getThisDescriptor(); \
	static TableDescriptor& getDescriptor(); \

#define IMPLEMENT_TABLE_BEGIN(class_name, table_name) \
	TableDescriptor& class_name::getThisDescriptor(){\
	return class_name::getDescriptor();\
}\
	TableDescriptor& class_name::getDescriptor()\
{\
	class_name* object_ = 0; \
	static FieldDescriptor s_##class_name##records[] = {

#define IMPLEMENT_RECORD_INFO(class_name, member_name, value_type_, default_value, key_type_) \
{ #member_name, default_value, offsetof(class_name, member_name), value_type_, key_type_ },

#define IMPLEMENT_TABLE_END(class_name, table_name) \
}; \
	static TableDescriptor s_##class_name##_descriptor = { s_##class_name##records, sizeof(s_##class_name##records) / sizeof(FieldDescriptor), table_name }; \
	return s_##class_name##_descriptor; \
}


#define SQL_CMD_COUNT			10240

#define cast_value(ptr, class_name)		*((class_name*)ptr)



void GetRecordValue(DBRecord* record, const FieldDescriptor& field, uint32 valuestrlength, int8* valuestr, uint32& valuesize);
void GetValueRecord(DBRecord* record, const FieldDescriptor& field, const int8* valuestr, uint32 valuesize);
bool GetQuerySqlCmd(char* sql_cmd, uint32& size, DBRecord* query_record, uint32 result_max_count, const std::string& compare_record_names, const std::string& return_record_names);
bool GetInsertSqlCmd(char* sql_cmd, uint32& size, DBRecord* insert_record, const std::string& compare_record_names);
bool GetUpdateSqlCmd(char* sql_cmd, int32& size, DBRecord* update_record, const std::string& compare_record_names, const std::string& update_record_names);
bool GetDeleteSqlCmd(char* sql_cmd, uint32& size, DBRecord* delete_record, const std::string& compare_record_names);

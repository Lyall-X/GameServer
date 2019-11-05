#include "mysql.hpp"
#include <mysql.h>

#define IS_CHAR_AVIALABLE(x) ((x <= 'z' && x >= 'a') || (x <= 'Z' && x >= 'A') || (x <= '9' && x >= '0') || (x == '_'))

IMPLEMENT_CLASS(DBRecord, EventDispatcher);

bool DBRecord::operator >> (BinaryStream& bytes)
{
	return bytes << id;
}

bool DBRecord::operator<<(BinaryStream& bytes)
{
	return bytes >> id;
}

bool HasRecord(const std::string& record_list, const std::string& record)
{
	std::string::size_type pos = record_list.find(record);
	return pos != std::string::npos;
}


void releaseResult(DBQueryResult* result)
{
	if (result == NULL)
		return;

	for (uint32 i = 0; i < result->length; ++i)
	{
		DBRowResult& rowResult = result->rows[i];
		for (uint32 j = 0; j < rowResult.fieldCount; ++j)
		{
			DBField& field = rowResult.fields[j];
			SAFE_DELETE_ARRAY(field.dataptr);
			field.length = 0;
		}
		SAFE_DELETE_ARRAY(rowResult.fields);
	}
	SAFE_DELETE_ARRAY(result->rows);
	SAFE_DELETE(result);
}

void GetRecordValue(DBRecord* record, const FieldDescriptor& field, uint32 valuestrlength, int8* valuestr, uint32& valuesize)
{
	char* dataptr = (char*)(record) + field.offset;

	switch (field.typevalue)
	{
	case Variant::TypeBoolean:
		sprintf_s(valuestr, valuestrlength, "%s", (dataptr[0] != 0) ? "TRUE" : "FALSE");
		valuesize = (uint32)strlen(valuestr);
		break;

	case Variant::TypeInt8:
		sprintf_s(valuestr, valuestrlength, "%ld", cast_value(dataptr, int8));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeUint8:
		sprintf_s(valuestr, valuestrlength, "%lu", cast_value(dataptr, uint8));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeInt16:
		sprintf_s(valuestr, valuestrlength, "%ld", cast_value(dataptr, int16));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeUint16:
		sprintf_s(valuestr, valuestrlength, "%lu", cast_value(dataptr, uint16));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeInt32:
		sprintf_s(valuestr, valuestrlength, "%ld", cast_value(dataptr, int32));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeUint32:
		sprintf_s(valuestr, valuestrlength, "%lu", cast_value(dataptr, uint32));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeInt64:
		sprintf_s(valuestr, valuestrlength, "%lld", cast_value(dataptr, int64));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeUint64:
		sprintf_s(valuestr, valuestrlength, "%llu", cast_value(dataptr, uint64));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeFloat32:
		sprintf_s(valuestr, valuestrlength, "%f", cast_value(dataptr, float));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeFloat64:
		sprintf_s(valuestr, valuestrlength, "%f", cast_value(dataptr, double));
		valuesize = (uint32)strlen(valuestr);
		break;
	case Variant::TypeString: {
			std::string* str = (std::string*)dataptr;
			valuesize = 0;
			strcpy_s(valuestr + valuesize, valuestrlength - valuesize, "\'");
			valuesize++;

			strcpy_s(valuestr + valuesize, valuestrlength - valuesize, str->c_str());
			valuesize += (uint32)str->length();

			strcpy_s(valuestr + valuesize, valuestrlength - valuesize, "\'");
			valuesize++;
			for (uint32 i = 1; i < valuesize - 1; i++)
			{
				if (valuestr[i] == '\'' || valuestr[i] == '\\')
				{
					memmove(valuestr + i + 1, valuestr + i, valuesize - i);
					valuestr[i] = '\\';
					i++;
					valuesize++;
				}
			}
		}
		break;
	case Variant::TypeDate:
		{
			time_t t = cast_value(dataptr, time_t);

			if (t == 0 && !strcmp(field.default, "CURRENT_TIMESTAMP"))
			{
				//如果缺省值是当前时间的话，则时间参数不填;
				memcpy(valuestr, "NULL", 4);
				valuesize = 4;
			}
			else
			{
				tm plt;
				localtime_s(&plt, &t);
				strftime(valuestr, valuestrlength, "'%Y-%m-%d %H:%M:%S'", &plt);
				valuesize = (uint32)strlen(valuestr);
			}
		}
		break;
	case Variant::TypeMemory:
		{
			valuesize = 0;
			valuestr[valuesize++] = '\'';

			BinaryStream* buffer = (BinaryStream*)dataptr;
			if (buffer->wpos() > 0)
				valuesize += mysql_escape_string(valuestr + valuesize, (char*)buffer->datas(), buffer->wpos());
			valuestr[valuesize++] = '\'';
		}
		break;
	}
	valuestr[valuesize] = 0;
}

void GetValueRecord(DBRecord* record, const FieldDescriptor& field, const int8* valuestr, uint32 size)
{
	char* dataptr = (char*)record + field.offset;
	switch (field.typevalue)
	{
	case Variant::TypeBoolean:
		*((bool*)dataptr) = valuestr == NULL ? 0 : valuestr[0] == 0 ? false : true;
		break;
	case Variant::TypeInt8:
		*((int8*)dataptr) = valuestr == NULL ? 0 : (int8)strtol(valuestr, NULL, 10);
		break;
	case Variant::TypeUint8:
		*((uint8*)dataptr) = valuestr == NULL ? 0 : (uint8)strtoul(valuestr, NULL, 10);
		break;
	case Variant::TypeInt16:
		*((int16*)dataptr) = valuestr == NULL ? 0 : (int16)strtol(valuestr, NULL, 10);
		break;
	case Variant::TypeUint16:
		*((uint16*)dataptr) = valuestr == NULL ? 0 : (uint16)strtoul(valuestr, NULL, 10);
		break;
	case Variant::TypeInt32:
							
		*((int32*)dataptr) = valuestr == NULL ? 0 : (int32)strtol(valuestr, NULL, 10);
		break;
	case Variant::TypeUint32:
								
		*((uint32*)dataptr) = valuestr == NULL ? 0 : (uint32)strtoul(valuestr, NULL, 10);
		break;
	case Variant::TypeInt64:
							
		*((int64*)dataptr) = valuestr == NULL ? 0 : (int64)_strtoi64(valuestr, NULL, 10);
		break;
	case Variant::TypeUint64:
							
		*((uint64*)dataptr) = valuestr == NULL ? 0 : (uint64)_strtoui64(valuestr, NULL, 10);
		break;
	case Variant::TypeFloat32:
							
		*((float*)dataptr) = valuestr == NULL ? 0 :  (float)strtod(valuestr, NULL);
		break;
	case Variant::TypeFloat64:
						
		*((double*)dataptr) = valuestr == NULL ? 0 : (double)strtod(valuestr, NULL);
		break;
	case Variant::TypeString:
		{
			if (!valuestr) break;
			std::string* str = (std::string*)dataptr;
			(*str) = valuestr;
		}
		break;
	case Variant::TypeDate:
		if (valuestr) {
			if (size <= 4 || valuestr[0] == 0 || memcmp(valuestr, "NULL", 4) == 0) {
				*((time_t*)dataptr) = 0;
			}
			else {
				tm tm1 = { 0 };
				strptime(valuestr, "%Y-%m-%d %H:%M:%S", &tm1);

				if (tm1.tm_year < 0) {
					*((time_t*)dataptr) = 0;
				}
				else {
					*((time_t*)dataptr) = mktime(&tm1);
				}
			}
		}
		break;
	case Variant::TypeMemory: {
			BinaryStream* buffer = (BinaryStream*)dataptr;
			buffer->clear();
			if (!valuestr)
				break;
			if (size <= 0)
				break;

			buffer->write(valuestr, size);
		}
		break;
	}
}

bool GetQuerySqlCmd(char* sql_cmd, uint32& size_, DBRecord* query_record, uint32 result_max_count, const std::string& compare_record_names, const std::string& return_record_names)
{
	TableDescriptor& descriptor = query_record->getThisDescriptor();
	char cmd_select_[SQL_CMD_COUNT] = { 0 };
	char cmd_where_[SQL_CMD_COUNT] = { 0 };
	char str_value_[SQL_CMD_COUNT] = { 0 };
	bool all_return = return_record_names.length() == 0 ? true : false;
	uint32 where_length_ = 0;
	for (uint32 i = 0; i < descriptor.recordCount; ++i)
	{
		FieldDescriptor& record = descriptor.records[i];

		if (all_return || HasRecord(return_record_names, record.field))
		{
			if (cmd_select_[0] == 0)
				sprintf_s(cmd_select_, SQL_CMD_COUNT, "select %s ", record.field);
			else
				sprintf_s(cmd_select_ + strlen(cmd_select_), SQL_CMD_COUNT - strlen(cmd_select_), ", %s ", record.field);
		}

		if (HasRecord(compare_record_names, record.field))
		{
			char value_[SQL_CMD_COUNT] = { 0 };
			uint32 value_size_ = 0;
			GetRecordValue(query_record, record, SQL_CMD_COUNT, value_, value_size_);
			if (!cmd_where_[0])
			{
				sprintf_s(cmd_where_, SQL_CMD_COUNT, "where %s=", record.field);
				where_length_ = (uint32)strlen(cmd_where_);
			}
			else
			{
				sprintf_s(cmd_where_ + where_length_, SQL_CMD_COUNT - where_length_, " and %s=", record.field);
				where_length_ += ((uint32)strlen(" and ") + (uint32)strlen(record.field) + (uint32)strlen("="));
			}

			memcpy(cmd_where_ + where_length_, value_, value_size_);

			where_length_ += value_size_;
		}
	}

	if (!cmd_where_[0])
		return false;

	sprintf_s(sql_cmd, SQL_CMD_COUNT, "%s from %s ", cmd_select_, descriptor.tableName);

	size_ = (uint32)strlen(sql_cmd) + where_length_;

	memcpy(sql_cmd + (uint32)strlen(sql_cmd), cmd_where_, where_length_);

	if (result_max_count > 0)
	{
		char limit_[64] = { 0 };
		sprintf_s(limit_, 64, " limit %d", result_max_count);
		memcpy(sql_cmd + size_, limit_, (uint32)strlen(limit_));
		size_ += (uint32)strlen(limit_);
	}
	sql_cmd[size_] = 0;
	return true;
}

bool GetInsertSqlCmd(int8* sql_cmd, uint32& size, DBRecord* insert_record, const std::string& compare_record_names)
{
	TableDescriptor& descriptor = insert_record->getThisDescriptor();

	char insert_value_[SQL_CMD_COUNT] = { 0 };
	char insert_coumns_[SQL_CMD_COUNT] = { 0 };
	uint32 insert_value_size_ = 0;

	char value_str[SQL_CMD_COUNT] = { 0 };
	uint32 value_str_size_ = 0;

	bool empty_compare = compare_record_names.length() == 0 ? true : false;
	
	for (uint32 i = 0; i < descriptor.recordCount; ++i)
	{
		value_str_size_ = 0;

		FieldDescriptor& record = descriptor.records[i];

		if (record.typekey == KeyTypeAutoInc) continue;

		if (!empty_compare)
		{
			if (!HasRecord(compare_record_names, record.field))
				continue;
		}

		if (!insert_coumns_[0])
			sprintf_s(insert_coumns_, SQL_CMD_COUNT, " (%s", record.field);
		else
			sprintf_s(insert_coumns_ + (uint32)strlen(insert_coumns_), SQL_CMD_COUNT - strlen(insert_coumns_), " ,%s", record.field);

		if (!insert_value_[0])
		{
			sprintf_s(insert_value_, SQL_CMD_COUNT, " value (");
			insert_value_size_ = (uint32)strlen(insert_value_);
		}
		else
		{
			sprintf_s(insert_value_ + insert_value_size_, SQL_CMD_COUNT - insert_value_size_, ", ");
			insert_value_size_ += (uint32)strlen(", ");
		}

		GetRecordValue(insert_record, record, SQL_CMD_COUNT, value_str, value_str_size_);
		//memcpy(insert_value_ + insert_value_size_, value_str, value_str_size_);
		memcpy_s(insert_value_ + insert_value_size_, SQL_CMD_COUNT, value_str, value_str_size_);
		insert_value_size_ += value_str_size_;
	}
	memcpy(insert_coumns_ + (uint32)strlen(insert_coumns_), ")\0", 2);

	memcpy(insert_value_ + insert_value_size_, ")", 1);
	insert_value_size_ += 1;

	sprintf_s(sql_cmd, SQL_CMD_COUNT, "insert %s %s", descriptor.tableName, insert_coumns_);
	size = (uint32)strlen(sql_cmd);

	memcpy(sql_cmd + size, insert_value_, insert_value_size_);
	size += insert_value_size_;

	sql_cmd[size] = 0;

	return true;
}

bool GetUpdateSqlCmd(char* sql_cmd, int32& size, DBRecord* update_record, const std::string& compare_record_names, const std::string& update_record_names)
{
	TableDescriptor& descriptor = update_record->getThisDescriptor();

	int8 set_value_[SQL_CMD_COUNT] = { 0 };
	int8 cmd_where_[SQL_CMD_COUNT] = { 0 };

	uint32 where_size_ = 0;
	uint32 set_size_ = 0;

	for (uint32 i = 0; i < descriptor.recordCount; ++i)
	{
		int8 value_[SQL_CMD_COUNT] = { 0 };
		uint32 value_size_ = 0;
		FieldDescriptor& record = descriptor.records[i];

		if (HasRecord(update_record_names, record.field))
		{
			GetRecordValue(update_record, record, SQL_CMD_COUNT, value_, value_size_);

			if (!set_value_[0])
			{
				sprintf_s(set_value_, SQL_CMD_COUNT, " set %s=", record.field);
				set_size_ = (uint32)strlen(set_value_);
			}
			else
			{
				sprintf_s(set_value_ + set_size_, SQL_CMD_COUNT - set_size_, ", %s=", record.field);
				set_size_ += ((uint32)strlen(", =") + (uint32)strlen(record.field));
			}

			memcpy(set_value_ + set_size_, value_, value_size_);

			set_size_ += value_size_;
		}

		if (HasRecord(compare_record_names, record.field))
		{
			GetRecordValue(update_record, record, SQL_CMD_COUNT, value_, value_size_);
			if (!cmd_where_[0])
			{
				sprintf_s(cmd_where_, SQL_CMD_COUNT, " where %s=", record.field);
				where_size_ = (uint32)strlen(cmd_where_);
			}
			else
			{
				sprintf_s(cmd_where_ + where_size_, SQL_CMD_COUNT - where_size_, " and %s=", record.field);
				where_size_ += ((uint32)strlen(" and ") + (uint32)strlen(record.field) + (uint32)strlen("="));
			}

			memcpy(cmd_where_ + where_size_, value_, value_size_);

			where_size_ += value_size_;
		}
	}

	if (!cmd_where_[0])
		return false;

	sprintf_s(sql_cmd, SQL_CMD_COUNT, "update %s", descriptor.tableName);
	size = (uint32)strlen(sql_cmd);

	memcpy(sql_cmd + size, set_value_, set_size_);
	size += set_size_;

	memcpy(sql_cmd + size, cmd_where_, where_size_);
	size += where_size_;

	sql_cmd[size] = 0;

	return true;
}

bool GetDeleteSqlCmd(char* sql_cmd, uint32& size, DBRecord* delete_record, const std::string& compare_record_names)
{
	TableDescriptor& descriptor = delete_record->getThisDescriptor();

	char cmd_where_[SQL_CMD_COUNT] = { 0 };
	int32 where_size_ = 0;

	char value_[SQL_CMD_COUNT] = { 0 };
	uint32 value_size_ = 0;

	for (uint32 i = 0; i < descriptor.recordCount; ++i)
	{
		FieldDescriptor& record = descriptor.records[i];
		if (HasRecord(compare_record_names, record.field))
		{
			GetRecordValue(delete_record, record, SQL_CMD_COUNT, value_, value_size_);

			if (!cmd_where_[0])
			{
				sprintf_s(cmd_where_, SQL_CMD_COUNT, " where %s=", record.field);
				where_size_ = (uint32)strlen(cmd_where_);
			}
			else
			{
				sprintf_s(cmd_where_ + where_size_, SQL_CMD_COUNT - where_size_, " and %s=", record.field);
				where_size_ += ((uint32)strlen(" and ") + (uint32)strlen(record.field) + (uint32)strlen("="));
			}

			memcpy(cmd_where_ + where_size_, value_, value_size_);

			where_size_ += value_size_;
		}
	}

	if (!cmd_where_[0])
		return false;

	sprintf_s(sql_cmd, SQL_CMD_COUNT, "delete from %s", descriptor.tableName);
	size = (uint32)strlen(sql_cmd);

	memcpy(sql_cmd + size, cmd_where_, where_size_);
	size += where_size_;

	sql_cmd[size] = 0;

	return true;
}


FieldDescriptor* TableDescriptor::getFieldDescriptor(const std::string& field) const
{
	for (uint32 i = 0; i < recordCount; ++i)
	{
		if (field == records[i].field)
			return &records[i];
	}

	return NULL;
}

DBResult::DBResult():
record(NULL)
{

}

DBResult::~DBResult()
{
	//SAFE_DELETE(record);
}

QueryResult::QueryResult():
records(NULL)
{
	//SAFE_DELETE_ARRAY(record);
}

QueryResult::~QueryResult()
{

}

InsertResult::InsertResult()
{

}

InsertResult::~InsertResult()
{

}

UpdateResult::UpdateResult():
updateRows(NULL)
{

}

UpdateResult::~UpdateResult()
{
	//SAFE_DELETE(updateRows);
}

DeleteResult::DeleteResult():
deleteRows(NULL)
{

}

DeleteResult::~DeleteResult()
{
	//SAFE_DELETE(deleteRows);
}

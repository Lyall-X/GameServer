#pragma once

class DBResult;
class QueryResult;
class InsertResult;
class UpdateResult;
class DeleteResult;
class DBRequest;

class DBEvent : public Event
{
public:
	DBEvent() {}
	~DBEvent() {}

public:
	static const char* QUERY;
	static const char* INSERT;
	static const char* UPDATE;
	static const char* REMOVE;
	static const char* DBEVENT_CMD;
public:
	DBResult* result;
	DBRequest* request;
};
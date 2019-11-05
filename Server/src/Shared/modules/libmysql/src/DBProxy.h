#pragma once

class DBConnector;
class DBProxy
{
public:
	DBProxy();
	~DBProxy();
	bool update(float time, float delay);
	DBConnector* connect(const char* ip, const char* user, const char* password, const char* name, int16 port);
protected:
	std::vector<DBConnector*> _connector;
private:
};
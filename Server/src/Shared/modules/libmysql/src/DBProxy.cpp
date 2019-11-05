#include "mysql.hpp"

DBConnector* DBProxy::connect(const char* ip, const char* user, const char* password, const char* name, int16 port)
{
	DBConnector* connector = new DBConnector;

	if (!connector->connect(ip, user, password, name, port))
	{
		delete connector;
		return NULL;
	}
	_connector.push_back(connector);
	return connector;
}

DBProxy::DBProxy()
{

}

DBProxy::~DBProxy()
{
	for (std::vector<DBConnector*>::iterator itr = _connector.begin();
		itr != _connector.end();
		++itr)
	{
		delete (*itr);
	}
	_connector.clear();
}

bool DBProxy::update(float time, float delay)
{
	for (std::vector<DBConnector*>::iterator itr = _connector.begin();
		itr != _connector.end();
		++itr)
	{
		(*itr)->update(time, delay);
	}

	return true;
}


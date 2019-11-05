#pragma once

template<class T> class Singleton
{
public:
	static T& Instance() { static T sT; return sT; }
};

#define  INSTANCE(class_name) Singleton<class_name>::Instance()
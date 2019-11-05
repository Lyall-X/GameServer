#pragma once

class Reference
{
public:
	Reference(void);
	Reference(const Reference& other);
	Reference& operator=(const Reference& other);
	virtual ~Reference(void);
	void incRef();
	void decRef();
protected:
	void deleteThis();
protected:
	static std::set<Reference*> setRefMgr;
	int mCount = 0;
};


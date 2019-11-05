#include "Shared.hpp"
#include "Reference.h"

std::set<Reference*> Reference::setRefMgr;
Reference::Reference(void):
mCount(0)
{
	setRefMgr.insert(this);
}

Reference::Reference(const Reference& other)
{
	*this = other;
	setRefMgr.insert(this);
}

Reference& Reference::operator=(const Reference& other)
{
	return *this;
}

Reference::~Reference(void)
{
	auto itr = setRefMgr.find(this);
	if (itr != setRefMgr.end()) {
		setRefMgr.erase(itr);
	}
	else
		LOG_ERROR("destruct error");
}

void Reference::incRef()
{
	mCount++;
}

void Reference::decRef()
{
	mCount--;
	if (mCount <= 0)
		this->deleteThis();
}

void Reference::deleteThis()
{
	delete this;
}



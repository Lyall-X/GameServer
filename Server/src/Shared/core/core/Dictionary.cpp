#include "Shared.hpp"
static Variant sVar;
Dictionary::Dictionary()
{

}

Dictionary::Dictionary(const Dictionary& other)
{
	for (auto itr : other.MapVariant())
	{
		Add((Variant&)itr.first, itr.second);
	}
}

Dictionary& Dictionary::operator=(const Dictionary& other)
{
	mMapVariant.clear();
	for (auto itr : other.MapVariant())
	{
		Add((Variant&)itr.first, itr.second);
	}
	return *this;
}

Dictionary::~Dictionary()
{

}

void Dictionary::Add(const Variant& key, const Variant& value)
{
	mMapVariant.insert(std::make_pair(key, value));
}

void Dictionary::Remove(const Variant& key)
{
	auto itr = mMapVariant.find(key);
	if (itr != mMapVariant.end())
		mMapVariant.erase(itr);
}

bool Dictionary::ContainsKey(const Variant& key)
{
	auto itr = mMapVariant.find(key);
	if (itr != mMapVariant.end())
		return true;

	return false;
}

Variant* Dictionary::GetVariant(const Variant& key)
{
	auto itr = mMapVariant.find(key);
	if (itr != mMapVariant.end())
		return &itr->second;
	return NULL;
}

Variant& Dictionary::operator[](const Variant& key)
{
	auto itr = mMapVariant.find(key);
	if (itr != mMapVariant.end())
		return itr->second;
	sVar.reset();
	return sVar;
}

void Dictionary::Clear()
{
	mMapVariant.clear();
}

bool Dictionary::operator >> (BinaryStream& bytes)
{
	CHECK_RETURN(bytes << (uint32)mMapVariant.size(), false);
	for (auto itr : mMapVariant)
	{
		CHECK_RETURN(bytes << itr.first, false);
		CHECK_RETURN(bytes << itr.second, false);
	}
	return true;
}

bool Dictionary::operator << (BinaryStream& bytes)
{
	uint32 count = 0;
	CHECK_RETURN(bytes >> count, false);
	for (uint32 i = 0; i < count; ++i)
	{
		Variant key;
		Variant value;
		CHECK_RETURN(bytes >> key, false);
		CHECK_RETURN(bytes >> value, false);
		Add(key, value);
	}
	return true;
}

#include "Shared.hpp"

RTTI Object::classObject("Object", sizeof(class Object), Object::createObject, NULL);

Object::Object(void)
{
}

Object::~Object(void)
{
}

Object* Object::createObject()
{
	return new Object;
}

RTTI* Object::getThisClass()
{
	return &classObject;
}

Object* Object::createInstance()
{
	return getThisClass()->createObject();
}

RTTI* Object::getBaseClass()
{
	return getThisClass()->mBaseClass;
}

const char* Object::getClassName()
{
	return getThisClass()->mClassName.c_str();
}

bool Object::operator<<(BinaryStream& bytes)
{
	return true;
}

bool Object::operator >> (BinaryStream& bytes)
{
	return true;
}


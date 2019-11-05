#pragma once

class Object;

typedef std::set<Object*> SetObject;
class ObjectManager
{
public:
	void Update();
	void FreeObject(Object* object);
protected:
	SetObject mSetObjects;
};
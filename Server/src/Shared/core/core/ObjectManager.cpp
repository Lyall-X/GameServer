#include "Shared.hpp"

void ObjectManager::Update()
{
	for (Object* obj : mSetObjects) {
		delete obj;
	}
	mSetObjects.clear();
}

void ObjectManager::FreeObject(Object* object)
{
	mSetObjects.insert(object);
}

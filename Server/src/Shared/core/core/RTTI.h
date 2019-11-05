#pragma once

class Object;

class RTTI
{
public:
	RTTI(cstring& className, int objectSize, void* pfnCreateObject, RTTI* baseClass);
	~RTTI(void);
	Object* createObject();
	static Object* createObject(cstring& className);
	static RTTI* getRTTI(cstring& className);
	static Object* dynamicCast(Object* object, cstring& className);
public:
	std::string mClassName;
	int mObjectSize;
	Object* (*mPfnCreateObject)();
	RTTI* mBaseClass;
};

#define DYNAMIC_CAST(object, class_name) RTTI::dynamicCast(object, #class_name)

#define DECLARE_CLASS(class_name) \
public: \
	static RTTI class##class_name; \
	static Object* createObject(); \
	virtual RTTI* getThisClass(); 


#define IMPLEMENT_CLASS(class_name, base_class_name) \
RTTI class_name::class##class_name(#class_name, sizeof(class class_name), class_name::createObject, &base_class_name::class##base_class_name); \
Object* class_name::createObject() { return new class_name; } \
RTTI* class_name::getThisClass() { return &class##class_name; }


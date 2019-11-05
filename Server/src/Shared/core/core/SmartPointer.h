#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

template<class T> class SmartPointer
{
public:
	inline SmartPointer(T* pObject = 0);
	inline SmartPointer(const SmartPointer &sp);
	inline virtual ~SmartPointer(void);
	inline T& operator*() const; 
	inline T* operator->() const;
	inline operator T*() const;
	inline SmartPointer& operator=(const SmartPointer& sp);
	inline SmartPointer& operator=(T* pObject);
	inline bool operator==(T* pObject);
	inline bool operator==(const SmartPointer& sp);
	inline bool operator!=(T* pObject);
	inline bool operator!=(const SmartPointer& sp);
private:
	T*			mObject;
};

#define SPointer(classname)	class classname; typedef SmartPointer<classname> classname##Ptr;

template<class T>
SmartPointer<T>::SmartPointer(const SmartPointer &sp)
{
	mObject = sp.mObject;
	if (mObject)
		mObject->incRef();
}

template<class T>
bool SmartPointer<T>::operator!=(const SmartPointer& sp)
{
	return (mObject != sp.mObject);
}

template<class T>
bool SmartPointer<T>::operator!=(T* object)
{
	return (mObject != object);
}

template<class T>
bool SmartPointer<T>::operator==(const SmartPointer& sp)
{
	return (mObject == sp.mObject);
}

template<class T>
bool SmartPointer<T>::operator==(T* object)
{
	return (mObject == object);
}

template<class T>
SmartPointer<T>& SmartPointer<T>::operator=(T* object)
{
	if (mObject)
		mObject->decRef();

	mObject = object;
	if (mObject)
		mObject->incRef();

	return *this;
}

template<class T>
SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer& sp)
{
	if (mObject)
		mObject->decRef();
	mObject = sp.mObject;
	if (mObject)
		mObject->incRef();

	return *this;
}

template<class T>
SmartPointer<T>::operator T*() const
{
	return mObject;
}

template<class T>
T* SmartPointer<T>::operator->() const
{
	return mObject;
}

template<class T>
T& SmartPointer<T>::operator*() const
{
	return *mObject;
}

template<class T>
SmartPointer<T>::SmartPointer(T* object /*= 0*/)
{
	mObject = object;
	if (mObject)
		mObject->incRef();
}

template<class T>
SmartPointer<T>::~SmartPointer(void)
{
	if (mObject)
		mObject->decRef();
}

#pragma warning(pop)

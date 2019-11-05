#pragma once

//
struct lua_State;
class Object;
class Player;

namespace luabind
{
	namespace detail {
		template<class T>
		struct delete_s;
		template<class T>
		struct destruct_only_s;
	}
};

#define LUA_ENUM(L, val)\
lua_pushliteral(L, #val);\
lua_pushnumber(L, val);\
lua_settable(L, -3)

class LuaScript
{
public:
	LuaScript(cstring& path);
	~LuaScript();
	bool loadScript(cstring& path);
	void bindScript();
	cstring& getName() { return mName; }
	lua_State* getLuaState() { return mLuaState; }
protected:
	lua_State* mLuaState;
	std::string mPath;
	std::string mName;
};

class LuaEngine
{
public:
	LuaEngine();
	~LuaEngine();
	void reloadScript();
//protected:

	LuaScript* loadScript(cstring& path);
	LuaScript* getScript(cstring& name);
	void clearScript();
public:
	static uint32 Call(Object* object, cstring& name, cstring& func);
	static uint32 Call(Object* object, cstring& name, cstring& func, int32 p1);
	static uint32 Call(Object* object, cstring& name, cstring& func, int32 p1, int32 p2);
	static uint32 Call(Object* object, cstring& name, cstring& func, int32 p1, int32 p2, int32 p3);
	static uint32 Call(Object* object, cstring& name, cstring& func, int32 p1, int32 p2, int32 p3, int32 p4);
	static uint32 Call(Object* object, cstring& name, cstring& func, cstring& p1);
	static uint32 Call(Object* object, cstring& name, cstring& func, cstring& p1, cstring& p2);
	static uint32 Call(Object* object, cstring& name, cstring& func, cstring& p1, cstring& p2, cstring& p3);
	static uint32 Call(Object* object, cstring& name, cstring& func, cstring& p1, cstring& p2, cstring& p3, cstring& p4);
	static uint32 Call(cstring& name, cstring& func, cstring& p1);
	static uint32 Call(cstring& name, cstring& func, cstring& p1, cstring& p2);
	static uint32 Call(cstring& name, cstring& func, cstring& p1, cstring& p2, cstring& p3);
	static uint32 Call(cstring& name, cstring& func, cstring& p1, cstring& p2, cstring& p3, cstring& p4);
	static uint32 Call(cstring& name, cstring& func, int32 p1);
	static uint32 Call(cstring& name, cstring& func, int32 p1, int32 p2);
	static uint32 Call(cstring& name, cstring& func, int32 p1, int32 p2, int32 p3);
	static uint32 Call(cstring& name, cstring& func, int32 p1, int32 p2, int32 p3, int32 p4);


	static int32 GetInt32(cstring& name, cstring& field);
	static bool SetInt32(cstring& name, cstring& field, const int32 value);
	static float64 GetFloat64(cstring& name, cstring& field);
	static bool SetFloat64(cstring& name, cstring& field, const float64 value);
	static std::string GetString(cstring& name, cstring& field);
	static bool SetString(cstring& name, cstring& field, cstring& value);


	template<class T>
	static bool GetValue(cstring& name, cstring& field, T& ret)
	{
		LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
		if (!luaScript)	return false;

		lua_State* luaState = luaScript->getLuaState();
		if (!luaState) return false;

		try {
			ret = luabind::object_cast<T>(luabind::globals(luaState)[field.c_str()]);
			return true;
		}
		catch (luabind::error& e)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), e.what());
			return false;
		}
		catch (const char* msg)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), msg);
			return false;
		}
		return false;
	}

	template<class T>
	static bool SetValue(cstring& name, cstring& field, const T& ret) {

		LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
		if (!luaScript)	return false;

		lua_State* luaState = luaScript->getLuaState();
		if (!luaState) return false;

		try {
			luabind::globals(luaState)[field.c_str()] = ret;
			return true;
		}
		catch (luabind::error& e)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), e.what());
			return false;
		}
		catch (const char* msg)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), msg);
			return false;
		}
		return false;
	}

	template <class... TArg>
	static uint32 CallScript(const std::string& name, const std::string& func, TArg&&... args)
	{
		LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
		if (!luaScript)	return false;

		lua_State* luaState = luaScript->getLuaState();
		if (!luaState) return false;

		try {
			int ret = luabind::call_function<int>(luaState, func.c_str(), std::forward<TArg>(args)...);
			return ret;
		}
		catch (luabind::error& e)
		{
			LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
			return 0;
		}
		catch (const char* msg)
		{
			LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
			return 0;
		}

		return 0;
	}

public:
	static Object* sCurrentObject;
protected:
	std::map<std::string, LuaScript*>		mMapLuaScript;
private:
};
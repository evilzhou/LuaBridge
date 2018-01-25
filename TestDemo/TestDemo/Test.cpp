#include <lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#pragma comment(lib, "lua.lib")

class CBase
{
public:
	static int Seq;

	CBase(int a)
	{
		A = a;
	}
	int A;

	int getA()
	{
		return A;
	}

	void setA(int a, lua_State *l)
	{
		A = a;
	}

	static int getSeq()
	{
		return Seq;
	}

	luabridge::LuaRef getDefault(luabridge::LuaRef key, lua_State *l)
	{
		int t = key.type();
		printf("get instance default prop %s\r\n", key.cast<std::string>().c_str());
		return luabridge::LuaRef(l);
	}

	void setDefault(luabridge::LuaRef key, luabridge::LuaRef value)
	{
		printf("set instance default prop %s %d\r\n", key.cast<std::string>().c_str(), value.cast<int>());
	}
};

int CBase::Seq = 666;


class CDemo : public CBase
{
public:
	CDemo(int b):CBase(b)
	{
		B = b;
	}
	int B;
};

static int num = 0;

static int getNum(lua_State *l)
{
	return num;
}

static void setNum(int n, lua_State* l)
{
	num = n;
}

static luabridge::LuaRef getDefault(luabridge::LuaRef key, lua_State *l)
{
	printf("get static default prop %s\r\n", key.cast<std::string>().c_str());
	return luabridge::LuaRef(l);
}

static void setDefault(luabridge::LuaRef key, luabridge::LuaRef value, lua_State *l)
{
	printf("set static default prop %s %d\r\n", key.cast<std::string>().c_str(), value.cast<int>());
}


int main()
{
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);

	luabridge::getGlobalNamespace(l)
		.beginNamespace("x")
			.beginClass<CBase>("CBase")
				.addConstructor<void(*)(int)>()
				.addProperty("A", &CBase::getA, &CBase::setA)
				.addStaticProperty("seq", &CBase::getSeq)
				.addPropertyDefault(&CBase::getDefault, &CBase::setDefault)
			.endClass()
			.addProperty("num", getNum)
			.addFunction("setNum", setNum)
			.addPropertyDefault(getDefault, setDefault)
		.endNamespace();

	luaL_dostring(l, R"+*(
print(x.num)
x.setNum(999)
print(x.num)
local t = x.CBase(111)
print(t.A)
t.A=666
print(t.A)
t.X=888
x.Z=999
print(x.W)
)+*");

	return 0;
}
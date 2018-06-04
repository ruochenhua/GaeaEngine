#pragma once
//lua script binding
struct lua_State;

class CScriptSystem
{
public:
	static lua_State *L;

public:
	CScriptSystem();
	~CScriptSystem();
private:
	lua_State *L;
};



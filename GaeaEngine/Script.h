#pragma once
//lua script binding
struct lua_State;

class CScriptSystem
{
public:
	CScriptSystem();
	~CScriptSystem();
private:
	lua_State *L;
};



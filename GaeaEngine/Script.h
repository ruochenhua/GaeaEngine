#pragma once
#include <LuaBridge/LuaBridge.h>
//lua script binding

class CScriptSystem
{
public:
	CScriptSystem();
	~CScriptSystem();

	void Init();
private:
	static lua_State * L;
};

CScriptSystem* GetScriptSystem();



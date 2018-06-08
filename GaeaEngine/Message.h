#pragma once
#include "Module.h"
#include <vector>

struct lua_State;

class CMessageManager : public CModule
{
public:
	CMessageManager();
	virtual void Update() override;

	int GetKeyDownQueue(lua_State* L);
	int GetKeyUpQueue(lua_State* L);

private:
	std::vector<int> m_KeyDownQueue;
	std::vector<int> m_KeyUpQueue;
	//todo mouse message
};
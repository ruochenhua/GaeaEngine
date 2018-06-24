#pragma once
#include "Module.h"
#include <vector>

struct lua_State;
struct SInputMsg 
{
	int type;	//up,down,motion(mouse)
	int key;

	//only mouse msg will need this
	int x;
	int y;
};

class CMessageManager : public CModule
{
public:
	CMessageManager();
	virtual void Update() override;

	int GetInputMsgQueue(lua_State *L);

private:
	std::vector<SInputMsg> m_InputMsgQueue;
};
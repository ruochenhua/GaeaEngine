#include "Message.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include "lua.hpp"

CMessageManager::CMessageManager()
{
	m_InputMsgQueue.reserve(100);
}

void CMessageManager::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		SInputMsg input_msg;
		input_msg.type = event.type;
		if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN)
		{
			input_msg.key = event.key.keysym.sym;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION)
		{
			input_msg.key = event.button.button;
		}
		else
		{
			input_msg.key = -1;
		}

		
		input_msg.x = event.button.x;
		input_msg.y = event.button.y;
		
		m_InputMsgQueue.push_back(input_msg);
	}
}

int CMessageManager::GetInputMsgQueue(lua_State* L)
{
	int queue_size = m_InputMsgQueue.size();
	for (int i = 0; i < queue_size; ++i)
	{
		lua_newtable(L);
		lua_pushnumber(L, m_InputMsgQueue[i].type);
		lua_setfield(L, -2, "type");
		lua_pushnumber(L, m_InputMsgQueue[i].key);
		lua_setfield(L, -2, "key");
		lua_pushnumber(L, m_InputMsgQueue[i].x);
		lua_setfield(L, -2, "x");
		lua_pushnumber(L, m_InputMsgQueue[i].y);
		lua_setfield(L, -2, "y");
	}

	m_InputMsgQueue.clear();
	return queue_size;
}
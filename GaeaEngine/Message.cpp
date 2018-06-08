#include "Message.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include "lua.hpp"

CMessageManager::CMessageManager()
{
	m_KeyDownQueue.reserve(100);
	m_KeyUpQueue.reserve(100);
}

void CMessageManager::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.key.type == SDL_KEYDOWN)
		{
			m_KeyDownQueue.push_back(event.key.keysym.sym);
		}

		if (event.key.type == SDL_KEYUP)
		{
			m_KeyUpQueue.push_back(event.key.keysym.sym);
		}
	}
}

int CMessageManager::GetKeyDownQueue(lua_State* L)
{
	int queue_size = m_KeyDownQueue.size();
	for (int i = 0; i < queue_size; ++i)
	{
		lua_pushnumber(L, m_KeyDownQueue[i]);
	}

	m_KeyDownQueue.clear();
	return queue_size;
}

int CMessageManager::GetKeyUpQueue(lua_State* L)
{
	int queue_size = m_KeyUpQueue.size();
	for (int i = 0; i < queue_size; ++i)
	{
		lua_pushnumber(L, m_KeyUpQueue[i]);
	}

	m_KeyUpQueue.clear();
	return queue_size;
}
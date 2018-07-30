#pragma once
#include <windows.h>
#include <memory>
#include <SDL.h>
#include <SDL_syswm.h>
#include <string>
#include <map>

typedef void(*MsgListener)(const SDL_Event& sdl_evt);

class CRenderWorld;
class CEntityManager;
class CModule;

class CRenderWindow
{
public:
	CRenderWindow();
	~CRenderWindow();

	//更新窗口
	bool Update();

	void AddModule(const std::string& module_name, CModule* module_ptr);

	CEntityManager* GetEntityManager();
public:
	static CEntityManager s_EntityMgr;
	static void InitLuaAPI(lua_State *L);

private:
	SDL_Window * m_Window;
	SDL_Surface *m_Surface;
	std::unique_ptr<CRenderWorld> m_RenderWorld;

	bool m_DoUpdate;

	//初始化renderworld
	void InitRenderWorld();
	//获取窗口的句柄
	HWND GetWindowHandle();
};
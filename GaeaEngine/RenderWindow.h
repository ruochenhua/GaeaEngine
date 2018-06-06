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

class CRenderWindow
{
public:
	CRenderWindow();
	~CRenderWindow();

	//更新窗口
	bool Update();

	//listener is a function pointer
	static bool RegisterMsgListener(const std::string& listener_name, MsgListener msg_listener);

	void GetEntityManager(CEntityManager *mgr);
	void AddEntityManager(CEntityManager *mgr);
private:
	SDL_Window * m_Window;
	SDL_Surface *m_Surface;
	std::unique_ptr<CRenderWorld> m_RenderWorld;

	bool m_DoUpdate;

	//初始化renderworld
	void InitRenderWorld();
	//获取窗口的句柄
	HWND GetWindowHandle();
	//按键信息， 暂时这么放一下
	void DispatchMsg();

	static std::map<std::string, MsgListener> s_MsgListenerMap;
};
#pragma once
#include <windows.h>
#include <memory>
#include <SDL.h>
#include <SDL_syswm.h>
#include <string>
#include <map>

class CRenderWorld;

typedef void(*MsgListener)(const SDL_Event& sdl_evt);

class CRenderWindow
{
public:
	CRenderWindow();
	~CRenderWindow();

	//���´���
	bool Update();

	//listener is a function pointer
	static bool RegisterMsgListener(const std::string& listener_name, MsgListener msg_listener);

private:
	SDL_Window * m_Window;
	SDL_Surface *m_Surface;
	std::unique_ptr<CRenderWorld> m_RenderWorld;

	bool m_DoUpdate;

	//��ʼ��renderworld
	void InitRenderWorld();
	//��ȡ���ڵľ��
	HWND GetWindowHandle();
	//������Ϣ�� ��ʱ��ô��һ��
	void DispatchMsg();

	static std::map<std::string, MsgListener> s_MsgListenerMap;
};

#include <iostream>

#include "RenderWindow.h"
#include "RenderWorld.h"

CRenderWindow::CRenderWindow()
{
	//初始化SDL2  
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	/* 创建窗口
	*第一个参数是窗口的标题
	*第二、三个参数指明了窗口的位置信息
	*第四、五个参数是窗口的大小（长、宽）
	*第六个参数是窗口的状态
	*SDL_WINDOW_SHOWN--显示窗口
	*SDL_WINDOW_BORDERLESS--显示无边框窗口
	*/
	m_Window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (!m_Window)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	//获取窗口的表面  
	m_Surface = SDL_GetWindowSurface(m_Window);
	if (!m_Surface)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	m_DoUpdate = true;

	InitRenderWorld();
}

CRenderWindow::~CRenderWindow()
{
	SDL_FreeSurface(m_Surface);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

HWND CRenderWindow::GetWindowHandle()
{
	if (!m_Window)
		return nullptr;

	//获取window的handle，关联Dx设备
	SDL_SysWMinfo systemInfo;
	SDL_VERSION(&systemInfo.version);
	SDL_GetWindowWMInfo(m_Window, &systemInfo);

	HWND handle = systemInfo.info.win.window;

	return handle;
}

bool CRenderWindow::Update()
{
	if (m_RenderWorld)
	{
		m_RenderWorld->Update(1.0f/60.0f);
	}


	DispatchMsg();
	return m_DoUpdate;
}

void CRenderWindow::DispatchMsg()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.key.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_DoUpdate = false;
		}

		auto iter = CRenderWindow::s_MsgListenerMap.begin();
		for (; iter != s_MsgListenerMap.end(); ++iter)
		{
			iter->second(event);
		}
	}
}

void CRenderWindow::InitRenderWorld()
{
	m_RenderWorld.reset(new CRenderWorld(GetWindowHandle()));
}

std::map<std::string, MsgListener> CRenderWindow::s_MsgListenerMap;
bool CRenderWindow::RegisterMsgListener(const std::string& listener_name, MsgListener msg_listener)
{
	s_MsgListenerMap.emplace(std::make_pair(listener_name, msg_listener));
	return true;
}
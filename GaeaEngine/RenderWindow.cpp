
#include <iostream>

#include "RenderWindow.h"
#include "RenderWorld.h"

CRenderWindow::CRenderWindow()
{
	//��ʼ��SDL2  
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	/* ��������
	*��һ�������Ǵ��ڵı���
	*�ڶ�����������ָ���˴��ڵ�λ����Ϣ
	*���ġ���������Ǵ��ڵĴ�С��������
	*�����������Ǵ��ڵ�״̬
	*SDL_WINDOW_SHOWN--��ʾ����
	*SDL_WINDOW_BORDERLESS--��ʾ�ޱ߿򴰿�
	*/
	m_Window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (!m_Window)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	//��ȡ���ڵı���  
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

	//��ȡwindow��handle������Dx�豸
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

	return m_DoUpdate;
}

void CRenderWindow::InitRenderWorld()
{
	m_RenderWorld.reset(new CRenderWorld(GetWindowHandle()));
}

void CRenderWindow::AddModule(const std::string& module_name, CModule* module_ptr)
{
	if (m_RenderWorld)
	{
		m_RenderWorld->AddModule(module_name, module_ptr);
	}
}
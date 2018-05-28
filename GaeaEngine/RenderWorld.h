#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <map>
#include <memory>

class CCameraManager;
class CEntityManager;
class CResourcesManager;

class CRenderWorld
{
public:
	static ID3D11Device *s_D3Ddevice;
	static ID3D11DeviceContext *s_D3DDeviceContext;
	
	static ID3D11Buffer *s_EntityTransformBuffer;

	static bool s_FirstDraw;
public:
	CRenderWorld(HWND g_HWnd);
	~CRenderWorld();
	
	void Update(double time_step);

private:

	IDXGISwapChain *m_SwapChain;//������  
	ID3D11RenderTargetView *m_RenderTargetView;//��ȾĿ����ͼ  

	ID3D11VertexShader *m_VS;
	ID3D11PixelShader *m_PS;

	ID3D11Texture2D* m_pDepthStencil;
	ID3D11DepthStencilView* m_pDepthStencilView;

	ID3D11InputLayout *m_Layout;    

	HRESULT InitDevice(HWND g_hWnd);
	HRESULT InitScene();

	void InitCamera(float w_div_h);

	void Render();
private:
	//managers
	std::unique_ptr<CCameraManager> m_CameraMgr;
	std::unique_ptr<CEntityManager> m_EntityMgr;
	std::unique_ptr<CResourcesManager> m_ResourceMgr;
};
#include "RenderWorld.h"
#include <iostream>
#include <assert.h>
#include "Camera.h"
#include "Resource.h"
#include "Entity.h"

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

D3DXMATRIX g_View;
D3DXMATRIX g_Projection;

ID3D11Device *CRenderWorld::s_D3Ddevice = nullptr;
ID3D11DeviceContext *CRenderWorld::s_D3DDeviceContext = nullptr;
ID3D11Buffer *CRenderWorld::s_EntityTransformBuffer = nullptr;

ID3D11Buffer* squareIndexBuffer;
ID3D11Buffer* squareVertBuffer;

bool CRenderWorld::s_FirstDraw = false;

struct  SConstantBuffer
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

unsigned int g_ScreenWidth;
unsigned int g_ScreenHeight;

CRenderWorld::CRenderWorld(HWND g_HWnd)
//	: m_SwapChain(nullptr), m_RenderTargetView(nullptr)
{
	RECT rect;
	GetClientRect(g_HWnd, &rect);
	g_ScreenWidth = rect.right - rect.left;
	g_ScreenHeight = rect.bottom - rect.top;

	assert(InitDevice(g_HWnd) == S_OK);
	assert(InitScene() == S_OK);
	
	GetResourceMgr()->InitBasicMesh();
}

CRenderWorld::~CRenderWorld()
{

#if defined(DEBUG) || defined(_DEBUG)  
	ID3D11Debug *d3dDebug;
	HRESULT hr = s_D3Ddevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	if (d3dDebug != nullptr)            d3dDebug->Release();
#endif  

	if (s_D3DDeviceContext)
	{
		s_D3DDeviceContext->ClearState();
		s_D3DDeviceContext->Release();
		s_D3DDeviceContext = nullptr;
	}

	if (m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = nullptr;
	}

	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = nullptr;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain = nullptr;
	}

	if (m_VS)
	{
		m_VS->Release();
		m_VS = nullptr;
	}

	if (m_PS)
	{
		m_PS->Release();
		m_PS = nullptr;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = nullptr;
	}
}

void CRenderWorld::PreUpdate()
{
	float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
	s_D3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, bgColor);

	//Refresh the Depth/Stencil view
	s_D3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void CRenderWorld::PostUpdate()
{
	//Present the backbuffer to the screen
	m_SwapChain->Present(0, 0);
}

HRESULT CRenderWorld::InitDevice(HWND g_hWnd)
{
	//Describe our SwapChain Buffer
	DXGI_MODE_DESC bufferDesc;
	HRESULT hr;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = g_ScreenWidth;
	bufferDesc.Height = g_ScreenHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = g_hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//Create our SwapChain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &s_D3Ddevice, NULL, &s_D3DDeviceContext);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	hr = s_D3Ddevice->CreateRenderTargetView(BackBuffer, NULL, &m_RenderTargetView);
	BackBuffer->Release();

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = g_ScreenWidth;
	depthStencilDesc.Height = g_ScreenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	s_D3Ddevice->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencil);
	s_D3Ddevice->CreateDepthStencilView(m_pDepthStencil, NULL, &m_pDepthStencilView);

	//Set our Render Target
	s_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_pDepthStencilView);
	return S_OK;
}

HRESULT CRenderWorld::InitScene()
{
	HRESULT hr;
	ID3D10Blob *vs, *ps;

	D3DX11CompileFromFile("..\\Assets\\Shaders\\HLSL\\shaders1.shader", 0, 0, "VS", "vs_4_0", 0, 0, 0, &vs, 0, 0);
	D3DX11CompileFromFile("..\\Assets\\Shaders\\HLSL\\shaders1.shader", 0, 0, "PS", "ps_4_0", 0, 0, 0, &ps, 0, 0);

	//Create the Shader Objects
	s_D3Ddevice->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &m_VS);
	s_D3Ddevice->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &m_PS);

	//Set Vertex and Pixel Shaders
	s_D3DDeviceContext->VSSetShader(m_VS, 0, 0);
	s_D3DDeviceContext->PSSetShader(m_PS, 0, 0);

	//Create the Input Layout
	hr = s_D3Ddevice->CreateInputLayout(layout, numElements, vs->GetBufferPointer(),
		vs->GetBufferSize(), &m_Layout);

	//Set the Input Layout
	s_D3DDeviceContext->IASetInputLayout(m_Layout);

	//Set Primitive Topology
	s_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = g_ScreenWidth;
	viewport.Height = g_ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	s_D3DDeviceContext->RSSetViewports(1, &viewport);

	//Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(D3DXMATRIX);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = s_D3Ddevice->CreateBuffer(&cbbd, NULL, &s_EntityTransformBuffer);

	return S_OK;
}

void CRenderWorld::Update(double time_step)
{
	PreUpdate();

	for (auto iter : m_ModuleMap)
	{
		iter.second->Update();
	}

	CCamera* main_cam = m_CameraMgr->GetCamera("MainCam");
	if (main_cam)
	{
		main_cam->GetLookAtMatrix(g_View);
		main_cam->GetPerspectiveFovMatrix(g_Projection);
	}

	PostUpdate();
}

void CRenderWorld::AddModule(const std::string& module_name, CModule* module_ptr)
{
	m_ModuleMap.emplace(module_name, module_ptr);
}
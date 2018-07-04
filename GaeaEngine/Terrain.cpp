#include <d3d11.h>

#include "Terrain.h"
#include "RenderWorld.h"
#include "Camera.h"
#include "Entity.h"

CTerrain::CTerrain(unsigned height, unsigned width)
	: m_Height(height), m_Width(width)
	, m_Pos(0.0f,0.0f,0.0f), m_Scale(1.0f, 1.0f)
{
	m_HeightMap.resize(m_Height * m_Width);
	
	//generate random object
	for (int i = 0; i < m_Height*m_Width; ++i)
	{
		m_HeightMap[i] = rand() % 10;
	}

	InitBuffer();
}

void CTerrain::Render()
{
	if (CCameraManager::s_MainCam)
	{
		D3DXMATRIX look_mat, pers_mat;
		CCameraManager::s_MainCam->GetLookAtMatrix(look_mat);
		CCameraManager::s_MainCam->GetPerspectiveFovMatrix(pers_mat);

		D3DXMATRIX trans_matrix;
		D3DXMatrixTransformation(&trans_matrix, 0, 0, 0, 0, 0, &m_Pos);

		UINT stride = sizeof(SVERTEX);
		UINT offset = 0;
		CRenderWorld::s_D3DDeviceContext->IASetVertexBuffers(0, 1, &m_Vb, &stride, &offset);

		CRenderWorld::s_D3DDeviceContext->IASetIndexBuffer(m_Ib, DXGI_FORMAT_R16_UINT, 0);

		SEntityTransConstBuffer et_buffer;
		D3DXMatrixTranspose(&et_buffer.world, &trans_matrix);
		D3DXMatrixTranspose(&et_buffer.view, &look_mat);
		D3DXMatrixTranspose(&et_buffer.project, &pers_mat);

		CRenderWorld::s_D3DDeviceContext->UpdateSubresource(CRenderWorld::s_EntityTransformBuffer, 0, nullptr, &et_buffer, 0, 0);
		CRenderWorld::s_D3DDeviceContext->VSSetConstantBuffers(0, 1, &CRenderWorld::s_EntityTransformBuffer);

		// draw the vertex buffer to the back buffer

		CRenderWorld::s_D3DDeviceContext->DrawIndexed(m_IndexRaw.size(), 0, 0);
	}
}

void CTerrain::InitBuffer()
{
	m_VertexRaw.resize(m_Height * m_Width);
	m_IndexRaw.reserve((m_Height-1)*(m_Width-1)*2*3);

	//initalize vertex buffer
	for (int i = 0; i < m_Height; ++i)
	{
		for (int j = 0; j < m_Width; ++j)
		{
			SVERTEX vertex;
			vertex.color = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
			vertex.normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//position is the point of the smallest point
			vertex.position = D3DXVECTOR3(j*m_Scale.x, m_HeightMap[i*m_Width + j], i*m_Scale.y);

			m_VertexRaw[i*m_Width + j] = vertex;
		}
	}

	for (int i = 0; i < m_Height - 1; ++i)
	{
		for (int j = 0; j < m_Width - 1; ++j)
		{
			m_IndexRaw.push_back(i*m_Width + j);
			m_IndexRaw.push_back((i + 1)*m_Width + j);
			m_IndexRaw.push_back(i*m_Width + j+1);

			m_IndexRaw.push_back((i + 1)*m_Width + j);
			m_IndexRaw.push_back((i + 1)*m_Width + j + 1);
			m_IndexRaw.push_back(i*m_Width + j + 1);
		}
	}

	//create buffer
	HRESULT hr;
	D3D11_BUFFER_DESC v_bd;
	ZeroMemory(&v_bd, sizeof(v_bd));

	unsigned int vert_size = sizeof(SVERTEX) * m_VertexRaw.size();
	v_bd.Usage = D3D11_USAGE_IMMUTABLE;
	v_bd.ByteWidth = vert_size;
	v_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       
	v_bd.CPUAccessFlags = 0;

	//设定用于初始化缓冲的数据	//
	D3D11_SUBRESOURCE_DATA init_vb_data;
	init_vb_data.pSysMem = &m_VertexRaw[0];

	//这里就不需要去map和unmap了，因为是IMMUTABLE
	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&v_bd, &init_vb_data, &m_Vb);

	//index buffer
	D3D11_BUFFER_DESC i_bd;
	ZeroMemory(&i_bd, sizeof(i_bd));

	unsigned int idx_size = sizeof(uint16_t) * m_IndexRaw.size();
	i_bd.Usage = D3D11_USAGE_DEFAULT;
	i_bd.ByteWidth = idx_size;
	i_bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	i_bd.CPUAccessFlags = 0;
	i_bd.MiscFlags = 0;
	i_bd.StructureByteStride = 0;

	//设定用于初始化索引缓冲的数据	
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = &m_IndexRaw[0];

	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&i_bd, &init_data, &m_Ib);
	
	CRenderWorld::s_D3DDeviceContext->IASetIndexBuffer(m_Ib, DXGI_FORMAT_R16_UINT, 0);
}

CTerrainManager::CTerrainManager()
{
	m_Terrain.reset(new CTerrain(100, 50));
}

void CTerrainManager::Update()
{
	if (m_Terrain)
	{
		m_Terrain->Render();
	}
}
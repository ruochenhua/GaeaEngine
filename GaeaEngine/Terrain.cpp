#include <d3d11.h>

#include "lua.hpp"
#include "Terrain.h"
#include "RenderWorld.h"
#include "Camera.h"
#include "Entity.h"

CTerrain::CTerrain(unsigned height, unsigned width, const D3DXVECTOR2& scale, const std::vector<float>& heightmap)
	: m_Height(height), m_Width(width)
	, m_Pos(0.0f,0.0f,0.0f), m_Scale(scale)
{
	m_HeightMap = heightmap;
	
	InitBuffer();
}

CTerrain::~CTerrain()
{
	m_Vb->Release();
	m_Vb = nullptr;


	m_Ib->Release();
	m_Ib = nullptr;
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
			vertex.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//position is the point of the smallest point
			vertex.position = D3DXVECTOR3(j*m_Scale.x, m_HeightMap[i*m_Width + j], i*m_Scale.y);

			m_VertexRaw[i*m_Width + j] = vertex;
		}
	}

	for (int i = 0; i < m_Height - 1; ++i)
	{
		for (int j = 0; j < m_Width - 1; ++j)
		{
			unsigned idx_0 = i * m_Width + j;
			unsigned idx_1 = (i + 1)*m_Width + j;
			unsigned idx_2 = i * m_Width + j + 1;
			m_IndexRaw.push_back(idx_0);
			m_IndexRaw.push_back(idx_1);
			m_IndexRaw.push_back(idx_2);

			//calculate normal
			D3DXVECTOR3 edge0, edge1, normal;
			D3DXVec3Subtract(&edge0, &m_VertexRaw[idx_0].position, &m_VertexRaw[idx_1].position);
			D3DXVec3Subtract(&edge1, &m_VertexRaw[idx_1].position, &m_VertexRaw[idx_2].position);
			D3DXVec3Cross(&normal, &edge0, &edge1);

			D3DXVec3Add(&m_VertexRaw[idx_0].normal, &m_VertexRaw[idx_0].normal, &normal);
			D3DXVec3Add(&m_VertexRaw[idx_1].normal, &m_VertexRaw[idx_1].normal, &normal);
			D3DXVec3Add(&m_VertexRaw[idx_2].normal, &m_VertexRaw[idx_2].normal, &normal);


			idx_0 = (i + 1)*m_Width + j;
			idx_1 = (i + 1)*m_Width + j + 1;
			idx_2 = i * m_Width + j + 1;
			m_IndexRaw.push_back(idx_0);
			m_IndexRaw.push_back(idx_1);
			m_IndexRaw.push_back(idx_2);

			D3DXVec3Subtract(&edge0, &m_VertexRaw[idx_0].position, &m_VertexRaw[idx_1].position);
			D3DXVec3Subtract(&edge1, &m_VertexRaw[idx_1].position, &m_VertexRaw[idx_2].position);
			D3DXVec3Cross(&normal, &edge0, &edge1);

			D3DXVec3Add(&m_VertexRaw[idx_0].normal, &m_VertexRaw[idx_0].normal, &normal);
			D3DXVec3Add(&m_VertexRaw[idx_1].normal, &m_VertexRaw[idx_1].normal, &normal);
			D3DXVec3Add(&m_VertexRaw[idx_2].normal, &m_VertexRaw[idx_2].normal, &normal);
		}
	}

	//normalize normal
	for (int i = 0; i < m_Height; ++i)
	{
		for (int j = 0; j < m_Width; ++j)
		{
			int idx = i * m_Width + j;
			D3DXVec3Normalize(&m_VertexRaw[idx].normal, &m_VertexRaw[idx].normal);
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

}

void CTerrainManager::Update()
{
	if (m_Terrain)
	{
		m_Terrain->Render();
	}
}

int CTerrainManager::CreateTerrain(lua_State* L)
{
	unsigned height = lua_tonumber(L, 3);
	float scale_h = lua_tonumber(L, 5);
	unsigned width = lua_tonumber(L, 4);
	float scale_w = lua_tonumber(L, 6);
	lua_pop(L, 4);

	unsigned map_size = height * width;

	std::vector<float> height_map(map_size);
	for (unsigned int i = 0; i < map_size; ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -2);
		float hm = lua_tonumber(L, -1);
		height_map[i] = hm;
		lua_pop(L, 1);
	}
	//pop heightmap table
	lua_pop(L, 1);

	printf("get width & height %d, %d\n", width, height);

	m_Terrain.reset(new CTerrain(height, width, D3DXVECTOR2(scale_h, scale_w), height_map));

	return 0;
}
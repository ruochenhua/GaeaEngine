#include "Resource.h"
#include "RenderWorld.h"

SMeshData* CreatePredefinedCubeMesh()
{
	SVERTEX cube_vert[] =
	{
		{ -1.0f, -1.0f, -1.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) },
		{ -1.0f, +1.0f, -1.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ +1.0f, +1.0f, -1.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ +1.0f, -1.0f, -1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) },
		{ -1.0f, -1.0f, +1.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ -1.0f, +1.0f, +1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) },
		{ +1.0f, +1.0f, +1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f) },
		{ +1.0f, -1.0f, +1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) },
	};

	DWORD cube_idx[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	std::vector<SVERTEX> vert_data(8);
	memcpy(&vert_data[0], &cube_vert[0], sizeof(SVERTEX) * 8);

	std::vector<DWORD> idx_data(36);
	memcpy(&idx_data[0], &cube_idx[0], sizeof(DWORD) * 36);

	SMeshData* cube_mesh = new SMeshData(vert_data, idx_data);

	return cube_mesh;
}

std::map<std::string, std::shared_ptr<SMeshData>> CResourcesManager::m_MeshMap;

SMeshData::SMeshData(const std::vector<SVERTEX>& vertices, const std::vector<DWORD>& indices)
	: vert_data(vertices), idx_data(indices)
{
	HRESULT hr;
	//vertex buffer
	D3D11_BUFFER_DESC v_bd;
	ZeroMemory(&v_bd, sizeof(v_bd));

	unsigned int vert_size = sizeof(SVERTEX) * vertices.size();
	v_bd.Usage = D3D11_USAGE_DEFAULT;                // write access, access by CPU and GPU
	v_bd.ByteWidth = sizeof(SVERTEX) * vertices.size();             
	v_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	v_bd.CPUAccessFlags = 0;    // allow CPU to write in buffer
	
	//设定用于初始化缓冲的数据	//
	D3D11_SUBRESOURCE_DATA init_vb_data;
	init_vb_data.pSysMem = &vertices[0];

	//这里就不需要去map和unmap了，因为是IMMUTABLE
	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&v_bd, &init_vb_data, &vb);

	UINT stride = sizeof(SVERTEX);
	UINT offset = 0;
	CRenderWorld::s_D3DDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	/*
	D3D11_MAPPED_SUBRESOURCE ms;
	CRenderWorld::s_DeviceContext->Map(vb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, &vertices[0], vert_size);                // copy the data
	CRenderWorld::s_DeviceContext->Unmap(vb, NULL);
	*/

	//index buffer
	D3D11_BUFFER_DESC i_bd;
	ZeroMemory(&i_bd, sizeof(i_bd));
	
	unsigned int idx_size = sizeof(DWORD) * indices.size();
	i_bd.Usage = D3D11_USAGE_DEFAULT;
	i_bd.ByteWidth = idx_size;
	i_bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	i_bd.CPUAccessFlags = 0;
	i_bd.MiscFlags = 0;
	i_bd.StructureByteStride = 0;

	//设定用于初始化索引缓冲的数据	
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = &indices[0];

	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&i_bd, &init_data, &ib);

	CRenderWorld::s_D3DDeviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

}

CResourcesManager::CResourcesManager()
{

}

std::shared_ptr<SMeshData> CResourcesManager::GetMeshData(const std::string& name)
{
	auto iter = m_MeshMap.find(name);
	if (iter != m_MeshMap.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void CResourcesManager::InitBasicMesh()
{
	//pre defined cube data;
	std::shared_ptr<SMeshData> cube_mesh;
	cube_mesh.reset(CreatePredefinedCubeMesh());

	m_MeshMap.emplace("cube", cube_mesh);
}

CResourcesManager g_ResMgr;
CResourcesManager* GetResourceMgr()
{
	return &g_ResMgr;
}
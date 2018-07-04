#include "Resource.h"
#include "RenderWorld.h"

std::map<std::string, std::shared_ptr<SMeshData>> CResourcesManager::m_MeshMap;

SMeshData::SMeshData(const std::vector<SVERTEX>& vertices, const std::vector<uint16_t>& indices)
	: vert_data(vertices), idx_data(indices)
{
	HRESULT hr;
	//vertex buffer
	D3D11_BUFFER_DESC v_bd;
	ZeroMemory(&v_bd, sizeof(v_bd));

	unsigned int vert_size = sizeof(SVERTEX) * vertices.size();
	v_bd.Usage = D3D11_USAGE_DEFAULT;                // write access, access by CPU and GPU
	v_bd.ByteWidth = vert_size;
	v_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	v_bd.CPUAccessFlags = 0;    // allow CPU to write in buffer
	
	//设定用于初始化缓冲的数据	//
	D3D11_SUBRESOURCE_DATA init_vb_data;
	init_vb_data.pSysMem = &vertices[0];

	//这里就不需要去map和unmap了，因为是IMMUTABLE
	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&v_bd, &init_vb_data, &vb);

	
	/*
	D3D11_MAPPED_SUBRESOURCE ms;
	CRenderWorld::s_DeviceContext->Map(vb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, &vertices[0], vert_size);                // copy the data
	CRenderWorld::s_DeviceContext->Unmap(vb, NULL);
	*/

	//index buffer
	D3D11_BUFFER_DESC i_bd;
	ZeroMemory(&i_bd, sizeof(i_bd));
	
	unsigned int idx_size = sizeof(uint16_t) * indices.size();
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
}

CResourcesManager::CResourcesManager()
{

}

bool CResourcesManager::LoadModel(const std::string& model_name, const std::string& file_path)
{
	std::vector<SPRIMITIVE> model_prim;
	std::vector<SVERTEX> model_vert;
	std::vector<uint16_t> model_idx;
	m_AssimpImporter.LoadFBXFile(file_path, model_prim, model_vert, model_idx);

	SMeshData* model_mesh = new SMeshData(model_vert, model_idx);
	m_MeshMap.emplace("Sting", model_mesh);
	return true;
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

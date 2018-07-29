#include "stdafx.h"
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
	
	//�趨���ڳ�ʼ�����������	//
	D3D11_SUBRESOURCE_DATA init_vb_data;
	init_vb_data.pSysMem = &vertices[0];

	//����Ͳ���Ҫȥmap��unmap�ˣ���Ϊ��IMMUTABLE
	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&v_bd, &init_vb_data, &vb);

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

	//�趨���ڳ�ʼ���������������	
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = &indices[0];

	hr = CRenderWorld::s_D3Ddevice->CreateBuffer(&i_bd, &init_data, &ib);

	//���Լ�����ͼ
	hr = D3DX11CreateShaderResourceViewFromFile(CRenderWorld::s_D3Ddevice, "../Assets/Models/sniper/textures/KSR29sniperrifle_Base_Color.jpg", NULL, NULL, &texture_view, NULL);


	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CRenderWorld::s_D3Ddevice->CreateSamplerState(&sampDesc, &sampler_state);
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
	m_MeshMap.emplace(model_name, model_mesh);
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

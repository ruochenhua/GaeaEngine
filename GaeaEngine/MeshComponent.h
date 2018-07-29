#pragma once
#include <vector>
#include <memory>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "Component.h"
#include "AssimpImporter.h"

struct SMeshData
{
	//for now all immutable vertex buffer
	SMeshData(const std::vector<SVERTEX>& vertices,
		const std::vector<uint16_t>& indices);
	//for now, default to vertex and index data
	//raw data
	std::vector<SVERTEX> vert_data;
	std::vector<uint16_t> idx_data;

	//buffer
	ID3D11Buffer *vb;
	ID3D11Buffer *ib;

	ID3D11Resource* texture;
	ID3D11ShaderResourceView *texture_view;

	ID3D11SamplerState *sampler_state;
};

class CMeshComponent : public CComponent
{
public:
	CMeshComponent(const std::string& file_path);

public:
	static void InitLuaAPI(lua_State *L);

private:
	std::shared_ptr<SMeshData> m_MeshData;
};
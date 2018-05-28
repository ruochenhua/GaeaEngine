#pragma once
//manager mesh data, texture, normal, etc..
#include <memory>
#include <vector>
#include <map>
#include <d3dx9math.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

//struct of vertices
struct SVERTEX
{
	float x, y, z;
	D3DXCOLOR color;
};

struct SMeshData
{
	//for now all immutable vertex buffer
	SMeshData(const std::vector<SVERTEX>& vertices, 
		const std::vector<DWORD>& indices);
	//for now, default to vertex and index data
	//raw data
	std::vector<SVERTEX> vert_data;
	std::vector<DWORD> idx_data;

	//buffer
	ID3D11Buffer *vb;
	ID3D11Buffer *ib;
};

class CResourcesManager
{
public:
	static std::shared_ptr<SMeshData> GetMeshData(const std::string& name);
	CResourcesManager();

	//todo: mesh file loading using script(Lua)
private:

	//name(usually predefined or file path) and mesh data
	static std::map<std::string, std::shared_ptr<SMeshData>> m_MeshMap;
};
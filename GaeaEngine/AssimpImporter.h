#pragma once
#include <string>
#include <d3dx9math.h>
#include <vector>
//struct of vertices
struct SVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 texcoord0;

	D3DXCOLOR color;
};

struct SPRIMITIVE {
	std::string name;
	uint32_t m_startIndex;
	uint32_t m_numIndices;
	uint32_t m_startVertex;
	uint32_t m_numVertices;

	//Sphere m_sphere;
	//Aabb m_aabb;
	//Obb m_obb;
};

struct SMaterial
{
	std::string material_path;
	
	//diffuse, specular, ambient
	//normal map, light map, etc
};

struct SChunk {

	std::vector<SPRIMITIVE> primitives;

	//vb ib
	//raw data of vertex, and indices
	std::vector<SVERTEX> vertex_raw;
	std::vector<uint16_t> index_raw;
};

class CAssimpImporter
{
public:
	bool LoadFBXFile(const std::string& file_path, 
		std::vector<SPRIMITIVE>& primitive,
		std::vector<SVERTEX>& vertices, 
		std::vector<uint16_t>& indices);

private:
};
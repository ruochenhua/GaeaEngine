#pragma once
#include "Module.h"
#include <memory>
#include <vector>
#include <d3dx9math.h>

#include "AssimpImporter.h"

struct ID3D11Buffer;
struct lua_State;

class CTerrain
{
public:
	CTerrain(unsigned height, unsigned width, const D3DXVECTOR2& scale, const std::vector<float>& heightmap);
	~CTerrain();
	void Render();

private:
	//height map raw data
	unsigned m_Height, m_Width;
	std::vector<float> m_HeightMap;

	void InitBuffer();

	ID3D11Buffer *m_Vb;
	ID3D11Buffer *m_Ib;

	D3DXVECTOR3 m_Pos;
	D3DXVECTOR2 m_Scale;

	std::vector<SVERTEX> m_VertexRaw;
	std::vector<uint16_t> m_IndexRaw;
};

class CTerrainManager : public CModule
{
public:
	CTerrainManager();

	//mainly for rendering
	void Update() override;
	int CreateTerrain(lua_State* L);
private:
	//for now, only have one chunk
	std::unique_ptr<CTerrain> m_Terrain;
};
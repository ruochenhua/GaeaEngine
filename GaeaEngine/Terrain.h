#pragma once
#include "Module.h"
#include <memory>
#include <vector>
#include <d3dx9math.h>

#include "AssimpImporter.h"

struct ID3D11Buffer;

class CTerrain
{
public:
	CTerrain(unsigned height, unsigned width);
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
private:
	//for now, only have one chunk
	std::unique_ptr<CTerrain> m_Terrain;
};
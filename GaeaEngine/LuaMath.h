#pragma once
#include <d3dx9math.h>

namespace LuaMath 
{
	D3DXVECTOR3 Vec3Cross(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b)
	{
		D3DXVECTOR3 p_out;
		D3DXVec3Cross(&p_out, &a, &b);
		return p_out;
	}

	D3DXVECTOR3 Vec3Add(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b)
	{
		D3DXVECTOR3 p_out;
		D3DXVec3Add(&p_out, &a, &b);
		return p_out;
	}

	D3DXVECTOR3 Vec3Sub(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b)
	{
		D3DXVECTOR3 p_out;
		D3DXVec3Subtract(&p_out, &a, &b);
		return p_out;
	}
};



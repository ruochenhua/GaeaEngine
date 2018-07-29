#pragma once
#include <d3dx9math.h>
#include "Component.h"


struct Vec3Helper
{
	template <unsigned index>
	static float get(D3DXVECTOR3 const* vec)
	{
		switch (index)
		{
		case 1:
			return vec->x;
			break;
		case 2:
			return vec->y;
			break;
		case 3:
			return vec->z;
			break;
		default:
			return 0.0f;
			break;
		}
	}

	template <unsigned index>
	static void set(D3DXVECTOR3* vec, float value)
	{
		switch (index)
		{
		case 1:
			vec->x = value;
			break;
		case 2:
			vec->y = value;
			break;
		case 3:
			vec->z = value;
			break;
		default:
			break;
		}
	}
};

struct QuatHelper
{
	template <unsigned index>
	static float get(D3DXQUATERNION const* q)
	{
		switch (index)
		{
		case 1:
			return q->x;
			break;
		case 2:
			return q->y;
			break;
		case 3:
			return q->z;
			break;
		case 4:
			return q->w;
		default:
			return 0.0f;
			break;
		}
	}

	template <unsigned index>
	static void set(D3DXQUATERNION* q, float value)
	{
		switch (index)
		{
		case 1:
			q->x = value;
			break;
		case 2:
			q->y = value;
			break;
		case 3:
			q->z = value;
			break;
		case 4:
			q->w = value;
			break;
		default:
			break;
		}
	}
};


class CTransformComponent : public CComponent
{
public:
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXQUATERNION GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetTranslation() { return m_Transition; }

	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z, float w);
	void SetTranslation(float x, float y, float z);

public:
	static void InitLuaAPI(lua_State* L);
private:
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Transition;
	D3DXQUATERNION m_Rotation;
};
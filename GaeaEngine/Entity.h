#pragma once
#include <vector>
#include <memory>
#include <d3dx9math.h>
#include <tuple>

struct SMeshData;

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

class CEntity
{
public:
	void Render();
	
	//setter, getter
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXQUATERNION GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetTransition() { return m_Transition; }

	void SetScale(const D3DXVECTOR3& scale);
	void SetRotation(const D3DXQUATERNION& rot);
	void SetTransition(const D3DXVECTOR3& trans);

private:
	friend class CEntityManager;
	CEntity(unsigned int id, std::shared_ptr<SMeshData> mesh_data);

	unsigned int m_ID;
	std::shared_ptr<SMeshData> m_MeshData;
	
	D3DXVECTOR3 m_Scale;
	D3DXQUATERNION m_Rotation;
	D3DXVECTOR3 m_Transition;	
};

#include "Script.h"

class CEntityManager
{
public:
	CEntityManager();
	//return id of the entity
	unsigned int AddEntity(const std::string& mesh_name);
	bool RemoveEntity(unsigned int id);
	CEntity* GetEntity(unsigned int id);

	void Render();

private:
	std::vector<std::unique_ptr<CEntity>> m_Entities;
};
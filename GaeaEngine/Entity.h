#pragma once
#include <vector>
#include <memory>
#include <d3dx9math.h>

struct SMeshData;

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


class CEntityManager
{
public:
	CEntityManager();
	void Render();
	//return id of the entity
	unsigned int AddEntity(const std::string& mesh_name);

	bool RemoveEntity(unsigned int id);
	CEntity* GetEntity(unsigned int id);

private:
	std::vector<std::unique_ptr<CEntity>> m_Entities;
};
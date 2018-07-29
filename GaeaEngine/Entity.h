#pragma once
#include <vector>
#include <memory>
#include <tuple>
#include <d3dx9math.h>

#include "Module.h"
#include "Component.h"

struct SMeshData;
struct SEntityTransConstBuffer
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX project;
};

class CEntity
{
public:	
	~CEntity();
	bool AddComponent(const std::string& com_name, std::shared_ptr<CComponent> com_ptr);
	void RemoveComponent(const std::string& com_name);

public:
	static void InitLuaAPI(lua_State* L);

private:
	friend class CEntityManager;
	CEntity(uint32_t id);


	std::map<std::string, std::shared_ptr<CComponent>> m_Components;
	uint32_t m_ID;
};

//todo: 不应该是CModule
class CEntityManager
{
public:
	CEntityManager();
	//return id of the entity
	unsigned int AddEntity();
	bool RemoveEntity(unsigned int id);
	CEntity* GetEntity(unsigned int id);

	unsigned int GetMaxEntityNum() { return m_MaxEntityNum; }

public:
	static void InitLuaAPI(lua_State* L);

private:
	//上限暂时设定为100个
	unsigned int m_MaxEntityNum;
	std::vector<std::unique_ptr<CEntity>> m_Entities;
};
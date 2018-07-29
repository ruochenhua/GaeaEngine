#include "stdafx.h"

#include "Entity.h"
#include "Resource.h"
#include "RenderWorld.h"
#include "Script.h"

CEntity::CEntity(uint32_t id)
	: m_ID(id)
{
	//必须通过entitymanager制造以及销毁, 不开放luabridge接口
}

CEntity::~CEntity()
{

}

bool CEntity::AddComponent(const std::string& com_name, std::shared_ptr<CComponent> com_ptr)
{
	auto iter = m_Components.find(com_name);
	if (iter != m_Components.end())
	{
		//不支持重复的组件
		return false;
	}
	else
	{
		//新增组件
		m_Components.emplace(com_name, com_ptr);
		return true;
	}
}

void CEntity::RemoveComponent(const std::string& com_name)
{
	m_Components.erase(com_name);
}

void CEntity::InitLuaAPI(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		.beginClass<CEntity>("Entity")
		.addFunction("AddComponent", &CEntity::AddComponent)
		.addFunction("RemoveComponent", &CEntity::RemoveComponent)
		.endClass();

}
/*
void CEntity::Render()
{
	if (CCameraManager::s_MainCam)
	{	
		D3DXMATRIX look_mat, pers_mat;
		CCameraManager::s_MainCam->GetLookAtMatrix(look_mat);
		CCameraManager::s_MainCam->GetPerspectiveFovMatrix(pers_mat);

		D3DXMATRIX trans_matrix;
		D3DXMatrixTransformation(&trans_matrix, 0, 0, &m_Scale, 0, &m_Rotation, &m_Transition);

		UINT stride = sizeof(SVERTEX);
		UINT offset = 0;
		CRenderWorld::s_D3DDeviceContext->IASetVertexBuffers(0, 1, &m_MeshData->vb, &stride, &offset);

		CRenderWorld::s_D3DDeviceContext->IASetIndexBuffer(m_MeshData->ib, DXGI_FORMAT_R16_UINT, 0);

		SEntityTransConstBuffer et_buffer;
		D3DXMatrixTranspose(&et_buffer.world, &trans_matrix);
		D3DXMatrixTranspose(&et_buffer.view, &look_mat);
		D3DXMatrixTranspose(&et_buffer.project, &pers_mat);

		CRenderWorld::s_D3DDeviceContext->UpdateSubresource(CRenderWorld::s_EntityTransformBuffer, 0, nullptr, &et_buffer, 0, 0);
		CRenderWorld::s_D3DDeviceContext->VSSetConstantBuffers(0, 1, &CRenderWorld::s_EntityTransformBuffer);
		
		//texture
		CRenderWorld::s_D3DDeviceContext->PSSetShaderResources(0, 1, &m_MeshData->texture_view);
		CRenderWorld::s_D3DDeviceContext->PSSetSamplers(0, 1, &m_MeshData->sampler_state);


		// draw the vertex buffer to the back buffer
		CRenderWorld::s_D3DDeviceContext->DrawIndexed(m_MeshData->idx_data.size() , 0, 0);
	}
}
*/

CEntityManager::CEntityManager()
	: m_MaxEntityNum(100)
{
	m_Entities.resize(m_MaxEntityNum);
}

unsigned int CEntityManager::AddEntity()
{

	for (int i = 0; i < 100; ++i)
	{
		if (!m_Entities[i])
		{
			m_Entities[i].reset(new CEntity(i));
			return i;
		}
	}
	return -1;
}

bool CEntityManager::RemoveEntity(unsigned int id)
{
	if(id >= 0 && id < m_MaxEntityNum)
		m_Entities[id].reset();

	return true;
}

CEntity* CEntityManager::GetEntity(unsigned int id)
{
	if (id >= 0 && id < m_MaxEntityNum)
	{
		return m_Entities[id].get();
	}
	
	return nullptr;
}

void CEntityManager::InitLuaAPI(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		.beginClass<CEntityManager>("EntityManager")
		.addConstructor<void(*)(void)>()
		.addFunction("AddEntity", &CEntityManager::AddEntity)
		.addFunction("RemoveEntity", &CEntityManager::RemoveEntity)
		.addFunction("GetEntity", &CEntityManager::GetEntity)
		.addFunction("GetMaxEntityNum", &CEntityManager::GetMaxEntityNum)
		.endClass();
}

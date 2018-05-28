#include <assert.h>
#include "Entity.h"
#include "Resource.h"
#include "RenderWorld.h"
#include "Camera.h"

CEntity::CEntity(unsigned int id, std::shared_ptr<SMeshData> mesh_data)
	: m_ID(id), m_MeshData(mesh_data)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_Rotation(0.0f, 0.0f, 0.0f, 1.0f)
	, m_Transition(0.0f, 0.0f, 0.0f)
{

}

void CEntity::Render()
{
	if (CCameraManager::s_MainCam)
	{
		D3DXMATRIX look_mat, pers_mat;
		CCameraManager::s_MainCam->GetLookAtMatrix(look_mat);
		CCameraManager::s_MainCam->GetPerspectiveFovMatrix(pers_mat);

		D3DXMATRIX trans_matrix;
		D3DXMatrixTransformation(&trans_matrix, 0, 0, &m_Scale, 0, &m_Rotation, &m_Transition);
		D3DXMATRIX WVP = trans_matrix * look_mat * pers_mat;

		D3DXMatrixTranspose(&WVP, &WVP);
	
		CRenderWorld::s_D3DDeviceContext->UpdateSubresource(CRenderWorld::s_EntityTransformBuffer, 0, nullptr, &WVP, 0, 0);
		CRenderWorld::s_D3DDeviceContext->VSSetConstantBuffers(0, 1, &CRenderWorld::s_EntityTransformBuffer);


			// draw the vertex buffer to the back buffer
		CRenderWorld::s_D3DDeviceContext->DrawIndexed(36, 0, 0);
		//CRenderWorld::s_D3DDeviceContext->Draw(3, 0);
	}
}

void CEntity::SetScale(const D3DXVECTOR3& scale)
{
	m_Scale = scale;
}

void CEntity::SetRotation(const D3DXQUATERNION& rot)
{
	m_Rotation = rot;
}

void CEntity::SetTransition(const D3DXVECTOR3& trans)
{
	m_Transition = trans;
}

const int MAX_ENTITY_NUM = 100;
CEntityManager::CEntityManager()
{
	m_Entities.resize(100);
}

void CEntityManager::Render()
{
	for(auto &e : m_Entities)
	{
		if (e)
		{
			e->Render();
		}
	}
}

unsigned int CEntityManager::AddEntity(const std::string& mesh_name)
{
	for (int i = 0; i < 100; ++i)
	{
		if (!m_Entities[i])
		{
			auto mesh_data = CResourcesManager::GetMeshData(mesh_name);
			if (!mesh_data)
			{
				printf("mesh does not exist %s\n", mesh_name.data());
				return -1;
			}

			m_Entities[i].reset(new CEntity(i, mesh_data));
			return i;
		}
	}
	return -1;
}

bool CEntityManager::RemoveEntity(unsigned int id)
{
	if(id >= 0 && id < MAX_ENTITY_NUM)
		m_Entities[id].reset();

	return true;
}

CEntity* CEntityManager::GetEntity(unsigned int id)
{
	if (id >= 0 && id < MAX_ENTITY_NUM)
	{
		return m_Entities[id].get();
	}
	
	return nullptr;
}
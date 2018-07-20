#include <assert.h>
#include "Entity.h"
#include "Resource.h"
#include "RenderWorld.h"
#include "Camera.h"
#include "Script.h"

CEntity::CEntity(unsigned int id, std::shared_ptr<SMeshData> mesh_data)
	: m_ID(id), m_MeshData(mesh_data)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_Rotation(0.0f, 0.0f, 0.0f, 1.0f)
	, m_Transition(1.0f, 0.0f, 1.0f)
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

void CEntity::SetScale(float x, float y, float z)
{
	m_Scale = D3DXVECTOR3(x,y,z);
}

void CEntity::SetRotation(float x, float y, float z, float w)
{
	m_Rotation = D3DXQUATERNION(x,y,z,w);
}

void CEntity::SetTranslation(float x, float y, float z)
{
	m_Transition = D3DXVECTOR3(x,y,z);
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

void CEntityManager::Update()
{
	Render();
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


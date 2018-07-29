#include "stdafx.h"
#include "CameraModule.h"
#include "RenderWorld.h"
#include "Script.h"

std::map<std::string, uint32_t> g_CameraMap;

// entity id
uint32_t CCameraManager::s_MainCam;

D3DXVECTOR3 g_YUp(0.0, 1.0, 0.0);
D3DXVECTOR3 g_YDown(0.0, -1.0, 0.0);

/*
void CCamera::GetTransformMatrix(D3DXMATRIX& out_mat)
{
	//TODO: more later
	D3DXMatrixTranslation(&out_mat, m_Eye.x, m_Eye.y, m_Eye.z);
}

void CCamera::GetLookAtMatrix(D3DXMATRIX& out_mat)
{
	D3DXMatrixLookAtLH(&out_mat, &m_Eye, &m_At, &m_Up);
}

void CCamera::GetPerspectiveFovMatrix(D3DXMATRIX& out_mat)
{
	D3DXMatrixPerspectiveFovLH(&out_mat, m_CamPerspective.fov, m_CamPerspective.aspect_ratio,
		m_CamPerspective.near_clip, m_CamPerspective.far_clip);
}

void CCamera::Move(float x, float y, float z)
{
	D3DXVECTOR3 move_vec(x, y, z);
	D3DXVec3Add(&m_Eye, &m_Eye, &move_vec);
	D3DXVec3Add(&m_At, &m_At, &move_vec);
}

void CCamera::Rotate(float x, float y)
{	
	D3DXVECTOR3 dir, new_dir;
	D3DXVec3Subtract(&dir, &m_At, &m_Eye);
	D3DXVec3Normalize(&dir, &dir);

	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &dir, &m_Up);

	D3DXMATRIX right_rot;
	D3DXMatrixRotationAxis(&right_rot, &right, -y);
	//rot around right axis
	D3DXVec3TransformCoord(&new_dir, &dir, &right_rot);
	D3DXVec3Normalize(&new_dir, &new_dir);

	//limit the angle
	float up_limit = D3DXVec3Dot(&new_dir, &g_YUp);
	float down_limit = D3DXVec3Dot(&new_dir, &g_YDown);

	if (abs(up_limit) < 0.9 || abs(down_limit) < 0.9)
	{
		dir = new_dir;
	}

	D3DXMATRIX up_rot;
	D3DXMatrixRotationAxis(&up_rot, &m_Up, x);

	//rot around up axis
	D3DXVec3TransformCoord(&dir, &dir, &up_rot);
	D3DXVec3Add(&m_At, &m_Eye, &dir);
}
*/
CCameraManager::CCameraManager()
{

}

CCameraManager::~CCameraManager()
{

}

void CCameraManager::AddCamera(const char* cam_name, uint32_t entity_id)
{
	g_CameraMap.emplace(cam_name, entity_id);

	s_MainCam = entity_id;
}

void CCameraManager::RemoveCamera(const std::string& cam_name)
{
	//todo
}

void CCameraManager::SetMainCam(const std::string& name)
{
	auto iter = g_CameraMap.find(name);
	if (iter != g_CameraMap.end())
	{
		s_MainCam = iter->second;
	}
}

uint32_t CCameraManager::GetCamera(const std::string& name)
{
	auto iter = g_CameraMap.find(name);
	if (iter != g_CameraMap.end())
	{
		return iter->second;
	}
	else
	{
		return INVALID_ENTITY_ID;
	}
}

void CCameraManager::Update()
{
	if (s_MainCam && CRenderWorld::s_CameraTransformBuffer)
	{
		/*
		D3DXMATRIX trans_matrix, look_mat, pers_mat;
		CCameraManager::s_MainCam->GetTransformMatrix(trans_matrix);
		CCameraManager::s_MainCam->GetLookAtMatrix(look_mat);
		CCameraManager::s_MainCam->GetPerspectiveFovMatrix(pers_mat);
	
		D3DXMATRIX WVP = trans_matrix * look_mat * pers_mat;
		SCamConstBuffer cam_buffer;

		cam_buffer.cam_pos = s_MainCam->GetEye();
		CRenderWorld::s_D3DDeviceContext->UpdateSubresource(CRenderWorld::s_CameraTransformBuffer, 0, nullptr, &cam_buffer, 0, 0);
		CRenderWorld::s_D3DDeviceContext->VSSetConstantBuffers(1, 1, &CRenderWorld::s_CameraTransformBuffer);
		*/
	}
}

void CCameraManager::InitLuaAPI(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		.deriveClass<CCameraManager, CModule>("CameraManager")
		.addConstructor<void(*)(void)>()
		.addFunction("AddCamera", &CCameraManager::AddCamera)
		.addFunction("RemoveCamera", &CCameraManager::RemoveCamera)
		.addFunction("SetMainCam", &CCameraManager::SetMainCam)
		.addFunction("GetCamera", &CCameraManager::GetCamera)
		.endClass();
}
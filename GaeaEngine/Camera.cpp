#include <iostream>
#include "Camera.h"
#include "RenderWorld.h"

std::map<std::string, CCamera*> g_CameraMap;

CCamera *CCameraManager::s_MainCam = nullptr;

D3DXVECTOR3 g_Forward(1, 0, 0);
D3DXVECTOR3 g_Up(0, 1, 0);
D3DXVECTOR3 g_Right(0, 0, 1);

CCamera::CCamera()
	: m_Eye(0.0,0.0,0.0), m_At(1.0,0.0,0.0), m_Up(0.0, 1.0, 0.0)
{
	
}

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

void CCamera::SetEye(float x, float y, float z)
{
	m_Eye = D3DXVECTOR3(x, y, z);
}

void CCamera::SetAt(float x, float y, float z)
{
	m_At = D3DXVECTOR3(x, y, z);
}

void CCamera::SetUp(float x, float y, float z)
{
	m_Up = D3DXVECTOR3(x, y, z);
}

void CCamera::Move(float x, float y, float z)
{
	D3DXVECTOR3 move_vec(x, y, z);
	D3DXVec3Add(&m_Eye, &m_Eye, &move_vec);
	D3DXVec3Add(&m_At, &m_At, &move_vec);
}

void CCamera::Rotate(float x, float y)
{	
	D3DXVECTOR3 dir;
	D3DXVec3Subtract(&dir, &m_At, &m_Eye);
	D3DXVec3Normalize(&dir, &dir);

	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &dir, &m_Up);

	D3DXMATRIX right_rot;
	D3DXMatrixRotationAxis(&right_rot, &right, -y);
	//rot around right axis
	D3DXVec3TransformCoord(&dir, &dir, &right_rot);
	D3DXVec3TransformCoord(&m_Up, &m_Up, &right_rot);

	D3DXVec3Normalize(&m_Up, &m_Up);

	D3DXMATRIX up_rot;
	D3DXMatrixRotationAxis(&up_rot, &m_Up, x);

	//rot around up axis
	D3DXVec3TransformCoord(&dir, &dir, &up_rot);

	//printf("up %f %f %f, %f\n", right.x, right.y, right.z, D3DXVec3LengthSq(&right));
	D3DXVec3Add(&m_At, &m_Eye, &dir);
	
}

CCameraManager::CCameraManager()
{

}

CCameraManager::~CCameraManager()
{

}

void CCameraManager::AddCamera(const char* cam_name, CCamera *camera)
{
	g_CameraMap.emplace(cam_name, camera);

	s_MainCam = camera;
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

CCamera *CCameraManager::GetCamera(const std::string& name)
{
	auto iter = g_CameraMap.find(name);
	if (iter != g_CameraMap.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void CCameraManager::Update()
{
	if (s_MainCam && CRenderWorld::s_CameraTransformBuffer)
	{
		D3DXMATRIX trans_matrix, look_mat, pers_mat;
		CCameraManager::s_MainCam->GetTransformMatrix(trans_matrix);
		CCameraManager::s_MainCam->GetLookAtMatrix(look_mat);
		CCameraManager::s_MainCam->GetPerspectiveFovMatrix(pers_mat);
	
		D3DXMATRIX WVP = trans_matrix * look_mat * pers_mat;
		SCamConstBuffer cam_buffer;

		cam_buffer.cam_pos = s_MainCam->GetEye();
		CRenderWorld::s_D3DDeviceContext->UpdateSubresource(CRenderWorld::s_CameraTransformBuffer, 0, nullptr, &cam_buffer, 0, 0);
		CRenderWorld::s_D3DDeviceContext->VSSetConstantBuffers(1, 1, &CRenderWorld::s_CameraTransformBuffer);
	}
}
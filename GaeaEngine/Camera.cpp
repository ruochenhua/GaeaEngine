#include <iostream>
#include "Camera.h"

std::map<std::string, CCamera*> g_CameraMap;

CCamera *CCameraManager::s_MainCam = nullptr;

CCamera::CCamera()
	: m_Eye(0.0,0.0,1.0), m_At(0.0,0.0,0.0), m_Up(0.0, 1.0, 0.0)
{

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
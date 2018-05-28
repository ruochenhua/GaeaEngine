#include <iostream>
#include "Camera.h"

std::map<std::string, std::shared_ptr<CCamera>> g_CameraMap;

CCamera *CCameraManager::s_MainCam = nullptr;

CCamera::CCamera(const D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up, 
	const SCamPerspective& perspective)
	: m_Eye(eye), m_At(at), m_Up(up), m_CamPerspective(perspective)
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

void CCamera::Transform(const D3DXVECTOR3& move_dir)
{
	D3DXVec3Add(&m_Eye, &m_Eye, &move_dir);
}

void CameraMsgHandler(const SDL_Event& sdl_evt);
CCameraManager::CCameraManager()
{
	m_MsgListener = CameraMsgHandler;
	CRenderWindow::RegisterMsgListener("Camera", CameraMsgHandler);

	s_MainCam = nullptr;
}

CCameraManager::~CCameraManager()
{

}

void CameraMsgHandler(const SDL_Event& sdl_evt)
{
	const char* out_info = nullptr;
	D3DXVECTOR3 move_vec(0.0f, 0.0f, 0.0f);
	if (sdl_evt.key.type == SDL_KEYDOWN)
	{
		switch (sdl_evt.key.keysym.sym)
		{
		case SDLK_w:
			out_info = "W Key";
			move_vec.x = 1.0f;
			break;
		case SDLK_s:
			out_info = "S Key";
			move_vec.x = -1.0f;
			break;
		case SDLK_a:
			out_info = "A Key";
			move_vec.y = 1.0f;
			break;
		case SDLK_d:
			out_info = "D Key";
			move_vec.y = -1.0f;

			break;

		default:
			break;
		}
	}
	auto cam = CCameraManager::s_MainCam;
	if (cam)
	{
		cam->Transform(move_vec);
	}

	if (out_info)
	{
		std::cout << out_info << std::endl;
	}
}


void CCameraManager::AddCamera(const std::string& cam_name,
	D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up,
	const SCamPerspective& perspective)
{
	std::shared_ptr<CCamera> new_cam(new CCamera(eye, at, up, perspective));
	g_CameraMap.emplace(cam_name, new_cam);

	s_MainCam = new_cam.get();
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
		s_MainCam = iter->second.get();
	}
}

CCamera *CCameraManager::GetCamera(const std::string& name)
{
	auto iter = g_CameraMap.find(name);
	if (iter != g_CameraMap.end())
	{
		return iter->second.get();
	}
	else
	{
		return nullptr;
	}
}
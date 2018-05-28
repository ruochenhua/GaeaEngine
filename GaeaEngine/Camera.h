#pragma once
#include <d3dx9math.h>
#include <memory>
#include "RenderWindow.h"

struct SCamPerspective
{
	float fov, aspect_ratio, near_clip, far_clip;

	SCamPerspective() : fov(0.4f*3.14f), aspect_ratio(1.0f), near_clip(1.0f), far_clip(100.0f){}
};

class CCamera
{
public:
	CCamera(const D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up,
		const SCamPerspective& perspective);

	void GetLookAtMatrix(D3DXMATRIX& out_mat);
	void GetPerspectiveFovMatrix(D3DXMATRIX& out_mat);

	void Transform(const D3DXVECTOR3& vec);
private:
	//camera property
	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;

	SCamPerspective m_CamPerspective;
};

class CCameraManager
{
public:
	CCameraManager();
	~CCameraManager();

	void AddCamera(const std::string& cam_name, 
		D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up,
		const SCamPerspective& perspective);
	void RemoveCamera(const std::string& cam_name);
	
	static void SetMainCam(const std::string& name);
	static CCamera *s_MainCam;

	CCamera* GetCamera(const std::string& name);
private:
	MsgListener m_MsgListener;
};
	
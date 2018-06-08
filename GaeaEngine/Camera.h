#pragma once
#include <d3dx9math.h>
#include <memory>

#include "RenderWindow.h"
#include "Module.h"

struct SCamPerspective
{
	float fov, aspect_ratio, near_clip, far_clip;

	SCamPerspective() : fov(0.4f*3.14f), aspect_ratio(1.0f), near_clip(1.0f), far_clip(100.0f){}
};

class CCamera
{
public:
	CCamera();

	void GetLookAtMatrix(D3DXMATRIX& out_mat);
	void GetPerspectiveFovMatrix(D3DXMATRIX& out_mat);

	void SetEye(float x, float y, float z);
	void SetAt(float x, float y, float z);
	void SetUp(float x, float y, float z);

	D3DXVECTOR3 GetEye() { return m_Eye; }
	D3DXVECTOR3 GetAt() { return m_At; }
	D3DXVECTOR3 GetUp() { return m_Up; }

private:
	//camera property
	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;

	SCamPerspective m_CamPerspective;
};

class CCameraManager : public CModule
{
public:
	CCameraManager();
	~CCameraManager();

	void AddCamera(const char *cam_name, CCamera *camera);
	void RemoveCamera(const std::string& cam_name);
	
	void SetMainCam(const std::string& name);
	static CCamera *s_MainCam;

	CCamera* GetCamera(const std::string& name);
	virtual void Update() override {}
private:
	MsgListener m_MsgListener;
};
	
#pragma once
#include <d3dx9math.h>
#include <memory>
	
#include "RenderWindow.h"
#include "Module.h"

struct SCamConstBuffer
{
	D3DXVECTOR3 cam_pos;
};

class CCameraComponent;

class CCameraManager : public CModule
{
public:
	CCameraManager();
	~CCameraManager();

	void AddCamera(const char *cam_name, uint32_t entity_id);
	void RemoveCamera(const std::string& cam_name);
	
	void SetMainCam(const std::string& name);
	static uint32_t s_MainCam;

	uint32_t GetCamera(const std::string& name);
	virtual void Update() override;

public:
	static void InitLuaAPI(lua_State* L);
private:
	MsgListener m_MsgListener;
};
	
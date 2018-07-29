#pragma once
#include "Component.h"

//主要就是fov, aspect ratio等数据
class CCameraComponent : public CComponent
{
public:
	void SetFov(float fov) { m_Fov = fov; }
	void SetAspectRatio(float aspect_ratio) { m_AspectRatio = aspect_ratio; }
	void SetClippingPlanes(float n, float f) {
		m_Near = n;
		m_Far = f;
	}

	float GetFov() { return m_Fov; }
	float GetAspectRatio() { return m_AspectRatio; }
	float GetNearPlane() { return m_Near; }
	float GetFarPlane() { return m_Far; }

public:
	static void InitLuaAPI(lua_State *L);

private:
	float m_Fov;
	float m_AspectRatio;
	float m_Near;
	float m_Far;
};
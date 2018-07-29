#include "stdafx.h"
#include "CameraComponent.h"
#include "Script.h"

void CCameraComponent::InitLuaAPI(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.deriveClass<CCameraComponent, CComponent>("CameraComponent")
		.addConstructor<void(*)(void)>()
		.addFunction("SetFov", &CCameraComponent::SetFov)
		.addFunction("SetAspectRatio", &CCameraComponent::SetAspectRatio)
		.addFunction("SetClippingPlanes", &CCameraComponent::SetClippingPlanes)

		.addFunction("GetFov", &CCameraComponent::GetFov)
		.addFunction("GetAspectRatio", &CCameraComponent::GetAspectRatio)
		.addFunction("GetNearPlane", &CCameraComponent::GetNearPlane)
		.addFunction("GetFarPlane", &CCameraComponent::GetFarPlane)
		.endClass();
}
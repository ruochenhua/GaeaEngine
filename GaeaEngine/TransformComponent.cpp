#include "stdafx.h"
#include "TransformComponent.h"
#include "Script.h"

void CTransformComponent::SetScale(float x, float y, float z)
{
	m_Scale = D3DXVECTOR3(x, y, z);
}

void CTransformComponent::SetRotation(float x, float y, float z, float w)
{
	m_Rotation = D3DXQUATERNION(x, y, z, w);
}

void CTransformComponent::SetTranslation(float x, float y, float z)
{
	m_Transition = D3DXVECTOR3(x, y, z);
}

void CTransformComponent::InitLuaAPI(lua_State* L)
{
	//transform
	luabridge::getGlobalNamespace(L)
		//vec&quat
		.beginClass<D3DXVECTOR3>("D3DXVECTOR3")
		.addConstructor<void(*)(void)>()
		.addProperty("x", &Vec3Helper::get<1>, &Vec3Helper::set<1>)
		.addProperty("y", &Vec3Helper::get<2>, &Vec3Helper::set<2>)
		.addProperty("z", &Vec3Helper::get<3>, &Vec3Helper::set<3>)
		.endClass()

		.beginClass<D3DXQUATERNION>("D3DXQUATERNION")
		.addConstructor<void(*)(void)>()
		.addProperty("x", &QuatHelper::get<1>, &QuatHelper::set<1>)
		.addProperty("y", &QuatHelper::get<2>, &QuatHelper::set<2>)
		.addProperty("z", &QuatHelper::get<3>, &QuatHelper::set<3>)
		.addProperty("w", &QuatHelper::get<4>, &QuatHelper::set<4>)
		.endClass()

		//transform
		.deriveClass<CTransformComponent, CComponent>("CTransformComponent")
		.addConstructor<void(*)(void)>()
		.addFunction("GetScale", &CTransformComponent::GetScale)
		.addFunction("GetRotation", &CTransformComponent::GetRotation)
		.addFunction("GetTranslation", &CTransformComponent::GetTranslation)
		.addFunction("SetScale", &CTransformComponent::SetScale)
		.addFunction("SetRotation", &CTransformComponent::SetRotation)
		.addFunction("SetTranslation", &CTransformComponent::SetTranslation)
	.endClass();
}
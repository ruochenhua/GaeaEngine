#include "stdafx.h"
#include "ShaderComponent.h"
#include "Script.h"
#include "Resource.h"

CShaderComponent::CShaderComponent(const std::string& file_path)
{

}

void CShaderComponent::InitLuaAPI(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.deriveClass<CShaderComponent, CComponent>("ShaderComponent")
		.addConstructor<void(*)(const std::string&)>()
		.endClass();
}
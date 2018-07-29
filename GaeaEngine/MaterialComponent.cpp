#include "stdafx.h"
#include "MaterialComponent.h"
#include "Script.h"
#include "Resource.h"

CMaterialComponent::CMaterialComponent(const std::string& file_path)
{

}

void CMaterialComponent::InitLuaAPI(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.deriveClass<CMaterialComponent, CComponent>("MaterialComponent")
		.addConstructor<void(*)(const std::string&)>()
		.endClass();
}
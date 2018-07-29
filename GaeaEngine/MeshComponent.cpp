#include "stdafx.h"
#include "MeshComponent.h"
#include "Script.h"
#include "Resource.h"

CMeshComponent::CMeshComponent(const std::string& file_path)
{
	
}

void CMeshComponent::InitLuaAPI(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.deriveClass<CMeshComponent,CComponent>("MeshComponent")
		.addConstructor<void(*)(const std::string&)>()
		.endClass();
}
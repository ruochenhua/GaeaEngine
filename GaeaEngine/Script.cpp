#include "stdafx.h"

#include "Script.h"
#include "Entity.h"
#include "RenderWindow.h"
#include "Module.h"
#include "CameraModule.h"
#include "CameraComponent.h"
#include "Component.h"

#include "Message.h"
#include "Resource.h"
#include "LuaMath.h"
#include "Terrain.h"

#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"

clock_t GetTime()
{
	return clock();
}

int error_handle(lua_State *L)
{
	std::string error_msg = lua_tostring(L, -1);
	
	printf("%s\n", error_msg.data());
	assert(false);

	return 1;
}

lua_State*  CScriptSystem::L = nullptr;

CScriptSystem::CScriptSystem()
{

}

CScriptSystem::~CScriptSystem()
{
	lua_close(L);
}

void CScriptSystem::Init()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	luabridge::getGlobalNamespace(L)
		.addFunction("GetTime", GetTime)
		
		//组件和模块
		.beginClass<CModule>("Module")
		.endClass()

		.beginClass<CComponent>("Component")
		.endClass()

		.beginNamespace("MathHelper")
			.addFunction("Vec3Cross", LuaMath::Vec3Cross)
			.addFunction("Vec3Add", LuaMath::Vec3Add)
			.addFunction("Vec3Sub", LuaMath::Vec3Sub)
		.endNamespace()
		;

	CRenderWindow::InitLuaAPI(L);

	CEntity::InitLuaAPI(L);
	CTransformComponent::InitLuaAPI(L);
	CCameraComponent::InitLuaAPI(L);
	CMeshComponent::InitLuaAPI(L);
	CMaterialComponent::InitLuaAPI(L);
	CShaderComponent::InitLuaAPI(L);


	CEntityManager::InitLuaAPI(L);
	CMessageManager::InitLuaAPI(L);
	CCameraManager::InitLuaAPI(L);

	//camera_component
	//camera_module
	
	lua_pushcfunction(L, error_handle);
	luaL_loadfile(L, "../Assets/Scripts/main.lua");
	lua_pcall(L, 0, 0, -2);
}

CScriptSystem g_ScriptSys;
CScriptSystem* GetScriptSystem()
{
	return &g_ScriptSys;
}
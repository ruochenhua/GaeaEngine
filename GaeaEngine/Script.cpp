#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <assert.h>
#include <time.h>

#include "Script.h"
#include "Entity.h"
#include "RenderWindow.h"
#include "RenderWorld.h"
#include "Module.h"
#include "Camera.h"
#include "Message.h"
#include "Resource.h"
#include "LuaMath.h"
#include "Terrain.h"

clock_t GetTime()
{
	return clock();
}

CScriptSystem::CScriptSystem()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	luabridge::getGlobalNamespace(L)
		.addFunction("GetTime", GetTime)
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
		//Entity
		.beginClass<CEntity>("Entity")
		.addFunction("GetScale", &CEntity::GetScale)
		.addFunction("GetRotation", &CEntity::GetRotation)
		.addFunction("GetTranslation", &CEntity::GetTranslation)
		.addFunction("SetScale", &CEntity::SetScale)
		.addFunction("SetRotation", &CEntity::SetRotation)
		.addFunction("SetTranslation", &CEntity::SetTranslation)
		.endClass()

		.beginClass<CModule>("Module")
		.endClass()

		//ResourcesManager
		.deriveClass<CResourcesManager, CModule>("ResourcesManager")
		.addConstructor<void(*)(void)>()
		.addFunction("LoadModel", &CResourcesManager::LoadModel)
		.endClass()

		//EntityManager
		.deriveClass<CEntityManager, CModule>("EntityManager")
		.addConstructor<void(*)(void)>()
		.addFunction("AddEntity", &CEntityManager::AddEntity)
		.addFunction("RemoveEntity", &CEntityManager::RemoveEntity)
		.addFunction("GetEntity", &CEntityManager::GetEntity)
		.endClass()

		//camera
		.beginClass<CCamera>("Camera")
		.addConstructor<void(*)(void)>()
		.addFunction("GetEye", &CCamera::GetEye)
		.addFunction("GetAt", &CCamera::GetAt)
		.addFunction("GetUp", &CCamera::GetUp)
		.addFunction("SetEye", &CCamera::SetEye)
		.addFunction("SetAt", &CCamera::SetAt)
		.addFunction("SetUp", &CCamera::SetUp)
		.addFunction("Move", &CCamera::Move)
		.addFunction("Rotate", &CCamera::Rotate)
		.endClass()

		//camera manager
		.deriveClass<CCameraManager, CModule>("CameraManager")
		.addConstructor<void(*)(void)>()
		.addFunction("AddCamera", &CCameraManager::AddCamera)
		.addFunction("RemoveCamera", &CCameraManager::RemoveCamera)
		.addFunction("SetMainCam", &CCameraManager::SetMainCam)
		.addFunction("GetCamera", &CCameraManager::GetCamera)
		.addStaticData("MainCam", &CCameraManager::s_MainCam)
		.endClass()

		.deriveClass<CMessageManager, CModule>("MessageManager")
		.addConstructor<void(*)(void)>()
		.addCFunction("GetInputMsgQueue", &CMessageManager::GetInputMsgQueue)
		.endClass()

		//terrain manager
		.deriveClass<CTerrainManager, CModule>("TerrainManager")
		.addConstructor<void(*)(void)>()
		.addFunction("Update", &CTerrainManager::Update)
		.addCFunction("CreateTerrain", &CTerrainManager::CreateTerrain)
		.endClass()

		//render window
		.beginClass<CRenderWindow>("RenderWindow")
			.addConstructor<void(*)(void)>()
			.addFunction("Update", &CRenderWindow::Update)
			//.addFunction("GetEntityManager", &CRenderWindow::GetEntityManager)
			.addFunction("AddModule", &CRenderWindow::AddModule)
		.endClass()

		//math class
		.beginNamespace("MathHelper")
			.addFunction("Vec3Cross", LuaMath::Vec3Cross)
			.addFunction("Vec3Add", LuaMath::Vec3Add)
			.addFunction("Vec3Sub", LuaMath::Vec3Sub)
		.endNamespace()
		;
			

	luaL_dofile(L, "../Assets/Scripts/main.lua");
}

CScriptSystem::~CScriptSystem()
{
	lua_close(L);
}


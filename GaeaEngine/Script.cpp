#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <assert.h>

#include "Script.h"
#include "Entity.h"

static lua_State *L;

CScriptSystem::CScriptSystem()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//vec&quat
	luabridge::getGlobalNamespace(L)
		.beginClass<D3DXVECTOR3>("D3DXVECTOR3")
			.addProperty("x", &Vec3Helper::get<1>, &Vec3Helper::set<1>)
			.addProperty("y", &Vec3Helper::get<2>, &Vec3Helper::set<2>)
			.addProperty("z", &Vec3Helper::get<3>, &Vec3Helper::set<3>)
		.endClass()

		.beginClass<D3DXQUATERNION>("D3DXQUATERNION")
			.addProperty("x", &QuatHelper::get<1>, &QuatHelper::set<1>)
			.addProperty("y", &QuatHelper::get<2>, &QuatHelper::set<2>)
			.addProperty("z", &QuatHelper::get<3>, &QuatHelper::set<3>)
			.addProperty("w", &QuatHelper::get<4>, &QuatHelper::set<4>)
		.endClass()
	//Entity
	
		.beginClass<CEntity>("CEntity")
	//	.addFunction()
		.endClass()
	
	//EntityManager
	
		.beginClass<CEntityManager>("EntityManager")
		.addFunction("AddEntity", &CEntityManager::AddEntity)
		.addFunction("RemoveEntity", &CEntityManager::AddEntity)
		.addFunction("GetEntity", &CEntityManager::GetEntity)
		.endClass();
}

CScriptSystem::~CScriptSystem()
{
	lua_close(L);
}


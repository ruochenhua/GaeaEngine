#pragma once
#include "Component.h"

struct SShaderData
{

};

class CShaderComponent : public CComponent
{
public:
	CShaderComponent(const std::string& file_path);
public:
	static void InitLuaAPI(lua_State *L);

private:
	std::shared_ptr<SShaderData> m_ShaderData;
};

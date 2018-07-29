#pragma once
#include "Component.h"

struct SMaterialData
{

};

class CMaterialComponent : public CComponent
{
public:
	CMaterialComponent(const std::string& file_path);

public:
	static void InitLuaAPI(lua_State *L);

private:
	std::shared_ptr<SMaterialData> m_MaterialData;
};
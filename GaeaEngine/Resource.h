#pragma once
//manager mesh data, texture, normal, etc..
#include <memory>
#include <vector>
#include <map>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "Module.h"
#include "AssimpImporter.h"
#include "MeshComponent.h"


class CResourcesManager : public CModule
{
public:
	static std::shared_ptr<SMeshData> GetMeshData(const std::string& name);

	CResourcesManager();

	void Update() override {}
	bool LoadModel(const std::string& model_name, const std::string& file_path);
private:

	//name(usually predefined or file path) and mesh data
	static std::map<std::string, std::shared_ptr<SMeshData>> m_MeshMap;
	
	CAssimpImporter m_AssimpImporter;
};

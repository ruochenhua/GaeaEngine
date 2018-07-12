#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "AssimpImporter.h"

bool CAssimpImporter::LoadFBXFile(const std::string& file_path,
	std::vector<SPRIMITIVE>& primitives,
	std::vector<SVERTEX>& vertices, 
	std::vector<uint16_t>& indices)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType	|
		aiProcess_OptimizeMeshes	|
		aiProcess_PreTransformVertices);

	if (!scene)
	{
		return false;
	}

	uint16_t vertex_count = 0;
	uint32_t triangle_count = 0;
	uint16_t primitive_count = 0;

	int start_vertex = 0;
	int start_triangle = 0;

	aiNode* root_node = scene->mRootNode;
	
	if (scene->HasMeshes())
	{
		int mesh_count = scene->mNumMeshes;

		for (int i = 0; i < mesh_count; ++i)
		{
			aiMesh* a_mesh = scene->mMeshes[i];

			int vertex_size = a_mesh->mNumVertices;
			int face_size = a_mesh->mNumFaces;

			SPRIMITIVE prim;

			prim.name = std::string(a_mesh->mName.C_Str());
			prim.m_startIndex = start_triangle * 3;
			prim.m_startVertex = start_vertex;
			prim.m_numIndices = face_size * 3;
			prim.m_numVertices = vertex_size;

			vertex_count += vertex_size;
			triangle_count += face_size;

			printf("mesh no.%d, vertex size: %d, face size: %d\n", i, vertex_size, face_size);

			std::vector<SVERTEX> prim_vertices(vertex_size);
			std::vector<uint16_t> prim_indices(face_size*3 );
			for (int j = 0; j < vertex_size; ++j)
			{
				const aiVector3D& vert = a_mesh->mVertices[j];
				const aiVector3D& norm = a_mesh->mNormals[j];

				auto& vertex = prim_vertices[j];
				vertex.position.x = vert.x;
				vertex.position.y = vert.y;
				vertex.position.z = vert.z;

				vertex.normal.x = norm.x;
				vertex.normal.y = norm.y;
				vertex.normal.z = norm.z;

				if (a_mesh->HasTextureCoords(0))
				{
					vertex.texcoord0.x = a_mesh->mTextureCoords[0][j].x;
					vertex.texcoord0.y = a_mesh->mTextureCoords[0][j].y;
					vertex.texcoord0.z = a_mesh->mTextureCoords[0][j].z;
				}
				else
				{
					vertex.texcoord0.x = 0;
					vertex.texcoord0.y = 0;
					vertex.texcoord0.z = 0;
				}

				vertex.color = D3DXCOLOR(0, 1, 0, 1);
			}

			for (int j = 0; j < face_size; ++j)
			{
				const aiFace& face = a_mesh->mFaces[j];

				//	BX_CHECK(face.mNumIndices == 3, "Mesh must be triangulated");
				if (face.mNumIndices != 3)
				{
					continue;
				}

				prim_indices[j * 3] = face.mIndices[0] + start_vertex;
				prim_indices[j * 3 + 1] = face.mIndices[1] + start_vertex;
				prim_indices[j * 3 + 2] = face.mIndices[2] + start_vertex;

			}


			start_vertex += vertex_size;
			start_triangle += face_size;
			++primitive_count;

			vertices.insert(vertices.end(), prim_vertices.begin(), prim_vertices.end());
			indices.insert(indices.end(), prim_indices.begin(), prim_indices.end());
			primitives.push_back(prim);
			
		}
	}

	return true;
}
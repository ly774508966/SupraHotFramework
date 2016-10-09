// SupraHotModelConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelFormat.h"
#include <iostream>
#include "Serialization.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Test
{
	char* header = "SUPRA";

	float x;
	uint32 y;
	float z;

	uint32 dataAmount;
	float* floatData; // this could also be a void* array. then we would have to reintpret_cast<float*> it on read

	char* footer = "HOT";
};

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace SupraHot;
	using namespace Graphics;

	Assimp::Importer importer;
	uint32 flags =	aiProcess_CalcTangentSpace |
					aiProcess_Triangulate |
					aiProcess_JoinIdenticalVertices |
					//aiProcess_PreTransformVertices |
					aiProcess_RemoveRedundantMaterials |
					aiProcess_OptimizeMeshes;

	const aiScene* scene = importer.ReadFile("App-Content/Sponza.fbx", flags);
	std::vector<Mesh> loadedMeshes;


	for (uint32 i = 0, l = scene->mNumMeshes; i < l; ++i)
	{
		Mesh mesh;
		mesh.VertexAttributes = 0;

		const aiMesh& assimpMesh = *scene->mMeshes[i];

		// Set vertex attributes

		if (assimpMesh.HasPositions())
		{
			mesh.VertexAttributes += POSITION;
			mesh.VertexStride += 3 * sizeof(float);
		}

		if (assimpMesh.HasNormals())
		{
			mesh.VertexAttributes += NORMAL;
			mesh.VertexStride += 3 * sizeof(float);
		}

		if (assimpMesh.HasTextureCoords(0))
		{
			mesh.VertexAttributes += UV;
			mesh.VertexStride += 2 * sizeof(float);
		}

		if (assimpMesh.HasTangentsAndBitangents())
		{
			mesh.VertexAttributes += TANGENT;
			mesh.VertexStride += 3 * sizeof(float);

			mesh.VertexAttributes += BITANGENT;
			mesh.VertexStride += 3 * sizeof(float);

			// NOTE:
			// If we need binormals, we can easily compute them from the
			// normals and tangent. The formula is:
			// binormal = normal.cross(bitangent); 
			// This needs to be done for every vertex per Face (Triangle || Quad)
		}

		// Build up mesh
		mesh.Name = const_cast<char*>(assimpMesh.mName.C_Str());
		
		mesh.VertexCount = 0;
		mesh.IndexCount = 0;
		
		mesh.Vertices;
		mesh.Indices;

		// Build up materials.

		// Save texture paths

		/*aiMaterial& mat = *scene->mMaterials[assimpMesh.mMaterialIndex];
		aiString diffuseTexPath;
		mat.GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexPath);
		printf("Diffuse tex string: %s \n", diffuseTexPath.C_Str());*/

		printf("W Mesh name: %s \n", mesh.Name);
		loadedMeshes.push_back(mesh);
	}


	SHFModelFile modelFile;
	modelFile.MeshCount = scene->mNumMeshes;
	modelFile.Meshes = loadedMeshes.data();

	Serialization serialization("modelwritetest.bin");
	serialization.OpenFile(Serialization::WRITE_BINARY);
	serialization.WriteToFile(modelFile);
	serialization.CloseFile();
	
	printf("W mesh count: %d \n", modelFile.MeshCount);
	
	
	{
		SHFModelFile readModel;
		Serialization serialization1("modelwritetest.bin");
		serialization1.OpenFile(Serialization::READ_BINARY);
		serialization1.ReadFile(readModel);
		serialization1.CloseFile();

		printf("R mesh count: %d \n", readModel.MeshCount);
		for (uint32 i = 0, l = readModel.MeshCount; i < l; i++)
		{
			Mesh mesh = readModel.Meshes[i];
			printf("R Mesh name: %s \n", mesh.Name);
		}

	}

	return 0;
}


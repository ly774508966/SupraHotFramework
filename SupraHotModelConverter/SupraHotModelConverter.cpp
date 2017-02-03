// SupraHotModelConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelFormat.h"
#include <iostream>
#include "Serialization.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <sstream>
#include "StringUtils.h"

#define READ 0
#define BUILD_AS_TOOL 1


void ComputeAABB(const aiMesh* mesh, aiVector3D& min, aiVector3D& max)
{
	min = aiVector3D(10e10f, 10e10f, 10e10f);
	max = aiVector3D(-10e10f, -10e10f, -10e10f);
	for (uint32 i = 0; i < mesh->mNumVertices; ++i)
	{
		const aiVector3D v = mesh->mVertices[i];
		min = std::min(v, min);
		max = std::max(v, max);
	}
};

int _tmain(int argc, char* argv[])
{
	using namespace SupraHot;
	using namespace Graphics;

#if READ == 1
	{
		SHFModelFile readModel;
		Serialization serialization1("App-Content/Sponza.shfm");
		serialization1.OpenFile(Serialization::READ_BINARY);
		serialization1.ReadFile(readModel);
		serialization1.CloseFile();

		printf("(R) Header %s \n", readModel.Header.c_str());
		printf("(R) Mesh count : %d \n", readModel.MeshCount);
		printf("(R) Material count : %d \n", readModel.MaterialCount);
		printf("(R) Footer %s \n", readModel.Footer.c_str());

		for (uint32 i = 0; i < readModel.MaterialCount; i++)
		{
			Material& material = readModel.Materials[i];
			printf("(R) Material name: %s \n", material.Name.c_str());
			printf("(R) AlbedoMapPath: %s \n", material.AlbedoMapPath.c_str());
			printf("(R) NormalMapPath: %s \n", material.NormalMapPath.c_str());
			printf("(R) SpecularMapPath: %s \n", material.SpecularMapPath.c_str());
			printf("(R) ShininessReflectionMapPath: %s \n", material.ShininessReflectionMapPath.c_str());
			printf("(R) OpacityMapPath: %s \n", material.OpacityMapPath.c_str());
			printf("- - - - - - - - \n");
		}

		for (uint32 i = 0; i < readModel.MeshCount; i++)
		{
			Mesh& mesh = readModel.Meshes[i];
			printf("(R) Mesh name: %s \n", mesh.Name.c_str());
			printf("(R) Vertex count: %d \n", mesh.VertexCount);
			printf("(R) Index count: %d \n", mesh.IndexCount);
			printf("(R) Face count: %d \n", mesh.FaceCount);
			printf("(R) Element count: %d \n", mesh.ElementCount);
			printf("(R) Material name: %s \n", readModel.Materials[mesh.MaterialID].Name.c_str());
			printf("- - - - - - - - \n");
		}

	}
#endif

#if BUILD_AS_TOOL == 1

	if (argc < 2)
	{
		std::cout << "Usage: Supra Hot Model Converter (.fbx/.obj/.collada to SHFModelFormat(.shfm)) [output_filename]" << std::endl;
		return -1;
	}

	std::string path = argv[1];
	auto split = SplitString(path, "/\\");
	std::string s_Name = split.back();

	std::string outputPath;
	if (argc > 2)
	{
		outputPath = argv[2];
	}
	else
	{
		std::size_t found = s_Name.find_last_of(".");
		outputPath = s_Name.substr(0, found) + std::string(".shfm");
	}

	Assimp::Importer importer;
	/*uint32 flags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_PreTransformVertices |
		aiProcess_GenNormals |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_OptimizeMeshes;*/

	uint32 flags = aiProcessPreset_TargetRealtime_Quality;

	const aiScene* scene = importer.ReadFile(s_Name, flags);
	std::vector<Mesh> loadedMeshes;

	printf("Processing meshes now.... \n");

	std::vector<std::vector<float>> meshIDToVertexFloatData;
	std::vector<std::vector<uint32>> meshIDToIndexUint32Data;
	for (uint32 i = 0, l = scene->mNumMeshes; i < l; ++i)
	{
		Mesh mesh = {};
		mesh.VertexAttributes = 0;

		const aiMesh& assimpMesh = *scene->mMeshes[i];

		// Build up mesh
		mesh.Name = std::string(const_cast<char*>(assimpMesh.mName.C_Str()));
		mesh.NameLength = static_cast<uint32>(assimpMesh.mName.length);

		// Set vertex & index count
		mesh.VertexCount = assimpMesh.mNumVertices;
		mesh.FaceCount = assimpMesh.mNumFaces;
		mesh.IndexCount = assimpMesh.mNumFaces * 3;
		mesh.ElementCount = 0;

		// compute bounding box
		/*aiVector3D min;
		aiVector3D max;
		ComputeAABB(scene->mMeshes[i], min, max);*/

		std::vector<float> floatVertexData;
		std::vector<uint32> uint32IndexData;
		
		Vec3 meshCenter;
		float inverseTotal = 1.0f / mesh.VertexCount;

		// Set vertices and vertex attributes
		if (assimpMesh.HasPositions())
		{
			mesh.VertexAttributes += POSITION;
			mesh.VertexStride += 3;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				const aiVector3D& vertex = assimpMesh.mVertices[v];
				meshCenter.x += vertex.x;
				meshCenter.y += vertex.y;
				meshCenter.z += vertex.z;
			}

			meshCenter.x /= mesh.VertexCount;
			meshCenter.y /= mesh.VertexCount;
			meshCenter.z /= mesh.VertexCount;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				const aiVector3D& vertex = assimpMesh.mVertices[v];
				floatVertexData.push_back(vertex.x - meshCenter.x);
				floatVertexData.push_back(vertex.y - meshCenter.y);
				floatVertexData.push_back(vertex.z - meshCenter.z);
			}
		}

		if (assimpMesh.HasNormals())
		{
			mesh.VertexAttributes += NORMAL;
			mesh.VertexStride += 3;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				// todo: memcpy data directly into the floatVertexData vector!
				const aiVector3D& vertex = assimpMesh.mNormals[v];
				floatVertexData.push_back(vertex.x);
				floatVertexData.push_back(vertex.y);
				floatVertexData.push_back(vertex.z);
			}
		}

		if (assimpMesh.HasTextureCoords(0))
		{
			mesh.VertexAttributes += UV;
			mesh.VertexStride += 2;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				const aiVector3D& vertex = assimpMesh.mTextureCoords[0][v];
				floatVertexData.push_back(vertex.x);
				floatVertexData.push_back(vertex.y);
			}
		}

		if (assimpMesh.HasTangentsAndBitangents())
		{
			mesh.VertexAttributes += TANGENT;
			mesh.VertexStride += 3;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				const aiVector3D& vertex = assimpMesh.mTangents[v];
				floatVertexData.push_back(vertex.x);
				floatVertexData.push_back(vertex.y);
				floatVertexData.push_back(vertex.z);
			}

			mesh.VertexAttributes += BITANGENT;
			mesh.VertexStride += 3;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				const aiVector3D& vertex = assimpMesh.mBitangents[v];
				floatVertexData.push_back(vertex.x);
				floatVertexData.push_back(vertex.y);
				floatVertexData.push_back(vertex.z);
			}

			// NOTE:
			// If we need binormals, we can easily compute them from the
			// normals and tangent. The formula is:
			// binormal = normal.Cross(bitangent); 
			// This needs to be done for every vertex per Face (Triangle || Quad)
		}

		// get index data
		const uint64 numTriangles = assimpMesh.mNumFaces;
		for (uint64 faceIndex = 0; faceIndex < numTriangles; ++faceIndex)
		{
			const aiFace& triangleFace = assimpMesh.mFaces[faceIndex];

			//if (triangleFace.mNumIndices == 3)
			{
				uint32IndexData.push_back(triangleFace.mIndices[0]);
				uint32IndexData.push_back(triangleFace.mIndices[1]);
				uint32IndexData.push_back(triangleFace.mIndices[2]);
			}

		}

		mesh.IndexCountBytes = mesh.IndexCount * sizeof(uint32);
		mesh.ElementCount = mesh.VertexCount * mesh.VertexStride;
		mesh.ElementCountBytes = mesh.ElementCount * sizeof(float);
		mesh.VertexStrideBytes = mesh.VertexStride * sizeof(float);
		mesh.CenterPosition = meshCenter;

		meshIDToVertexFloatData.push_back(floatVertexData);
		meshIDToIndexUint32Data.push_back(uint32IndexData);

		mesh.Vertices = meshIDToVertexFloatData[i].data();
		mesh.Indices = meshIDToIndexUint32Data[i].data();

		loadedMeshes.push_back(mesh);
	}

	printf("Writing to file..... \n");

	SHFModelFile modelFile;
	modelFile.MeshCount = scene->mNumMeshes;
	modelFile.Meshes = loadedMeshes.data();

	Serialization serialization(outputPath);
	serialization.OpenFile(Serialization::WRITE_BINARY);
	serialization.WriteToFile(modelFile);
	serialization.CloseFile();

	printf("Done writing %d meshes to file.\n", modelFile.MeshCount);

#endif

	printf("Exiting... \n");
	return 0;
}


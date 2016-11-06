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
	uint32 flags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_PreTransformVertices |
		aiProcess_GenNormals |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_OptimizeMeshes;

	const aiScene* scene = importer.ReadFile(s_Name, flags);
	std::vector<Mesh> loadedMeshes;
	std::vector<Material> loadedMaterials;

	for (uint32 i = 0, l = scene->mNumMaterials; i < l; i++)
	{
		Material material = {};
		aiMaterial& mat = *scene->mMaterials[i];

		// Build up material
		aiString diffuseTexPath = {};
		aiString normalMapPath = {};
		aiString specularMapPath = {};
		aiString shininessReflectionMapPath = {};
		aiString opacityMapPath = {};

		// Diffuse map
		// Blender -> Texture -> Diffuse -> Color
		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexPath) == aiReturn_SUCCESS)
		{
			material.AlbedoMapPath = std::string(const_cast<char*>(diffuseTexPath.C_Str()));
			material.AlbeoMapPathLength = static_cast<uint32>(diffuseTexPath.length);
		}

		// Normal map
		// Blender -> Texture -> Geometry -> Normal
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &normalMapPath) == aiReturn_SUCCESS
			|| mat.GetTexture(aiTextureType_HEIGHT, 0, &normalMapPath) == aiReturn_SUCCESS)
		{
			material.NormalMapPath = std::string(const_cast<char*>(normalMapPath.C_Str()));
			material.NormalMapPathLength = static_cast<uint32>(normalMapPath.length);
		}

		// Roughness map
		// Blender -> Texture -> Specular -> Color
		if (mat.GetTexture(aiTextureType_SPECULAR, 0, &specularMapPath) == aiReturn_SUCCESS)
		{
			material.SpecularMapPath = std::string(const_cast<char*>(specularMapPath.C_Str()));
			material.SpecularMapPathLength = static_cast<uint32>(specularMapPath.length);
		}

		// Metalness map
		// Blender -> Texture -> Shading -> Mirror
		if (mat.GetTexture(aiTextureType_SHININESS, 0, &shininessReflectionMapPath) == aiReturn_SUCCESS
			|| mat.GetTexture(aiTextureType_REFLECTION, 0, &shininessReflectionMapPath) == aiReturn_SUCCESS)
		{
			material.ShininessReflectionMapPath = std::string(const_cast<char*>(shininessReflectionMapPath.C_Str()));
			material.ShininessReflectionMapPathLength = static_cast<uint32>(shininessReflectionMapPath.length);
		}

		// Alpha mask
		// Blender -> ....?
		if (mat.GetTexture(aiTextureType_OPACITY, 0, &opacityMapPath) == aiReturn_SUCCESS)
		{
			material.OpacityMapPath = std::string(const_cast<char*>(opacityMapPath.C_Str()));
			material.OpacityMapPathLength = static_cast<uint32>(opacityMapPath.length);
		}


		// Build up material properties such as color, name, etc.
		aiString materialName = {};
		mat.Get(AI_MATKEY_NAME, materialName);

		material.Name = std::string(const_cast<char*>(materialName.C_Str()));
		material.NameLength = static_cast<uint32>(material.Name.length());

		mat.Get(AI_MATKEY_SHININESS, material.Ns);

		// Temp color
		aiColor3D color;

		mat.Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.Ka.x = color.r;
		material.Ka.y = color.g;
		material.Ka.z = color.b;

		mat.Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.Kd.x = color.r;
		material.Kd.y = color.g;
		material.Kd.z = color.b;

		mat.Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.Ks.x = color.r;
		material.Ks.y = color.g;
		material.Ks.z = color.b;

		mat.Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material.Ke.x = color.r;
		material.Ke.y = color.g;
		material.Ke.z = color.b;

		material.ID = i;

		loadedMaterials.push_back(material);
	}

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
		mesh.MaterialID = assimpMesh.mMaterialIndex;

		// Set vertex & index count
		mesh.VertexCount = assimpMesh.mNumVertices;
		mesh.FaceCount = assimpMesh.mNumFaces;
		mesh.IndexCount = assimpMesh.mNumFaces * 3;
		mesh.ElementCount = 0;

		std::vector<float> floatVertexData;
		std::vector<uint32> uint32IndexData;

		// Set vertices and vertex attributes
		if (assimpMesh.HasPositions())
		{
			mesh.VertexAttributes += POSITION;
			mesh.VertexStride += 3;

			for (uint64 v = 0; v < mesh.VertexCount; ++v)
			{
				const aiVector3D& vertex = assimpMesh.mVertices[v];
				floatVertexData.push_back(vertex.x);
				floatVertexData.push_back(vertex.y);
				floatVertexData.push_back(vertex.z);
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
			// binormal = normal.cross(bitangent); 
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
	modelFile.MaterialCount = scene->mNumMaterials;
	modelFile.Materials = loadedMaterials.data();

	Serialization serialization(outputPath);
	serialization.OpenFile(Serialization::WRITE_BINARY);
	serialization.WriteToFile(modelFile);
	serialization.CloseFile();

	printf("Done writing %d meshes to file.\n", modelFile.MeshCount);

#endif

	printf("Exiting... \n");
	return 0;
}


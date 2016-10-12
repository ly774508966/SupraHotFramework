// SupraHotModelConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelFormat.h"
#include <iostream>
#include "Serialization.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define WRITE 1
#define READ 1


int _tmain(int argc, _TCHAR* argv[])
{
	using namespace SupraHot;
	using namespace Graphics;

#if WRITE == 1
	Assimp::Importer importer;
	uint32 flags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_PreTransformVertices |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_OptimizeMeshes;

	const aiScene* scene = importer.ReadFile("App-Content/Sponza.fbx", flags);
	std::vector<Mesh> loadedMeshes;
	std::vector<Material> loadedMaterials;

	for (uint32 i = 0, l = scene->mNumMaterials; i < l; i++)
	{
		Material material;
		aiMaterial& mat = *scene->mMaterials[i];

		// Build up material
		aiString diffuseTexPath;
		aiString normalMapPath;
		aiString specularMapPath;
		aiString shininessReflectionMapPath;
		aiString opacityMapPath;

		// Diffuse map
		// Blender -> Texture -> Diffuse -> Color
		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexPath) == aiReturn_SUCCESS)
		{
			material.AlbedoMapPath = diffuseTexPath.C_Str();
		}

		// Normal map
		// Blender -> Texture -> Geometry -> Normal
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &normalMapPath) == aiReturn_SUCCESS
			|| mat.GetTexture(aiTextureType_HEIGHT, 0, &normalMapPath) == aiReturn_SUCCESS)
		{
			material.NormalMapPath = normalMapPath.C_Str();
		}

		// Roughness map
		// Blender -> Texture -> Specular -> Color
		if (mat.GetTexture(aiTextureType_SPECULAR, 0, &specularMapPath) == aiReturn_SUCCESS)
		{
			material.SpecularMapPath = specularMapPath.C_Str();
		}

		// Metalness map
		// Blender -> Texture -> Shading -> Mirror
		if (mat.GetTexture(aiTextureType_SHININESS, 0, &shininessReflectionMapPath) == aiReturn_SUCCESS
			|| mat.GetTexture(aiTextureType_REFLECTION, 0, &shininessReflectionMapPath) == aiReturn_SUCCESS)
		{
			material.ShininessReflectionMapPath = shininessReflectionMapPath.C_Str();
		}

		// Alpha mask
		// Blender -> ....?
		if (mat.GetTexture(aiTextureType_OPACITY, 0, &opacityMapPath) == aiReturn_SUCCESS)
		{
			material.OpacityMapPath = opacityMapPath.C_Str();
		}

		// Build up material properties such as color, name, etc.
		aiString materialName;
		mat.Get(AI_MATKEY_NAME, materialName);
		material.Name = std::string(materialName.C_Str());

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


	std::vector<std::vector<float>> meshIDToVertexFloatData;
	for (uint32 i = 0, l = scene->mNumMeshes; i < l; ++i)
	{
		Mesh mesh;
		mesh.VertexAttributes = 0;

		const aiMesh& assimpMesh = *scene->mMeshes[i];


		// Build up mesh
		mesh.Name = assimpMesh.mName.C_Str();
		mesh.MaterialID = assimpMesh.mMaterialIndex;

		// Set vertex & index count
		mesh.VertexCount = assimpMesh.mNumVertices;
		mesh.FaceCount = assimpMesh.mNumFaces;
		mesh.IndexCount = assimpMesh.mNumFaces * 3;
		mesh.ElementCount = 0;

		std::vector<float> floatVertexData;

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

		mesh.IndexCountBytes = mesh.IndexCount * sizeof(uint32);
		mesh.ElementCount = mesh.VertexCount * mesh.VertexStride;
		mesh.ElementCountBytes = mesh.ElementCount * sizeof(float);
		mesh.VertexStrideBytes = mesh.VertexStride * sizeof(float);
		meshIDToVertexFloatData.push_back(floatVertexData);

		mesh.Vertices = meshIDToVertexFloatData[i].data();

		loadedMeshes.push_back(mesh);
	}


	SHFModelFile modelFile;
	modelFile.MeshCount = scene->mNumMeshes;
	modelFile.Meshes = loadedMeshes.data();
	modelFile.MaterialCount = scene->mNumMaterials;
	modelFile.Materials = loadedMaterials.data();

	Serialization serialization("modelwritetest.bin");
	serialization.OpenFile(Serialization::WRITE_BINARY);
	serialization.WriteToFile(modelFile);
	serialization.CloseFile();

	printf("W mesh count: %d \n", modelFile.MeshCount);
#endif


#if READ == 1
	{
		SHFModelFile readModel;
		Serialization serialization1("modelwritetest.bin");
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
		}

	}
#endif
	return 0;
}


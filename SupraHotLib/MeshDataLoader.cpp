#include "MeshDataLoader.h"
#include <unordered_map>
#include "StringUtil.h"
#include "Platform.h"
#include "FileSystem.h"
#include "SHFMBinaryLoader.h"
#include "ShaderLibrary.h"

namespace SupraHot
{
	namespace Utils
	{
		using namespace Math;
		using namespace Graphics;

		MeshDataLoader::MeshDataLoader()
		{
		}

		MeshDataLoader* MeshDataLoader::GetInstance()
		{
			static MeshDataLoader* instance(new MeshDataLoader);
			return instance;
		}

		MeshDataLoader::~MeshDataLoader()
		{
		}

		std::vector<MeshComponent*> MeshDataLoader::Load(std::string pathToSHFM)
		{
			MeshLoadData* meshLoadData = LoadSHFM(pathToSHFM);
			std::vector<MeshComponent*> meshComponents;
			
			for (size_t m = 0, l = meshLoadData->Meshes.size(); m < l; ++m)
			{
				MeshData* meshData = meshLoadData->Meshes[m];
				ShaderDescription* shaderDescription = ShaderLibrary::GetInstance()->GetShaderDescriptions()->at("MeshDefaultShader");
				ShaderMaterial* shaderMaterial = new ShaderMaterial(shaderDescription);
				Shader* shader = ShaderLibrary::GetInstance()->SelectShaderForShaderMaterialAndMeshData(meshData, shaderMaterial);
				shaderMaterial->SetShaderPermutation(shader);
				meshComponents.push_back(new MeshComponent(meshData, shaderMaterial));
			}

			delete meshLoadData;
			return meshComponents;
		}

		MeshLoadData* MeshDataLoader::LoadSHFM(std::string path)
		{
			SHFModelFile model = SHFMBinaryLoader::GetInstance().LoadFromFile(path);

			// Create map for materials
			std::vector<MeshData*> meshes {};

			for (uint32 i = 0; i < model.MeshCount; i++)
			{
				SHFModel::Mesh& modelMesh = model.Meshes[i];
				MeshData* meshData = new MeshData();

				meshData->Name = modelMesh.Name;
				meshData->FaceCount = modelMesh.FaceCount;
				meshData->VertexCount = modelMesh.VertexCount;

				// Changed this from the stupid combos
				// note: attri & bitfield == bitfield
				// == bitfield is redundant, since we just wanna know if the bit is > 1 (is set)
	
				if ((modelMesh.VertexAttributes & SHFModel::VertexBitfield::POSITION) == SHFModel::VertexBitfield::POSITION)
				{
					meshData->HasPositionData = true;
				}
				
				if ((modelMesh.VertexAttributes & SHFModel::VertexBitfield::NORMAL) == SHFModel::VertexBitfield::NORMAL)
				{
					meshData->HasNormalData = true;
				}
				
				if ((modelMesh.VertexAttributes & SHFModel::VertexBitfield::UV) == SHFModel::VertexBitfield::UV)
				{
					meshData->HasUVData = true;
				}
				
				if ((modelMesh.VertexAttributes & SHFModel::VertexBitfield::BITANGENT) == SHFModel::VertexBitfield::BITANGENT)
				{
					meshData->HasBiTangentData = true;
					meshData->HasTangentData = true;
				}

				// Generate vao
				glGenVertexArrays(1, &meshData->VAOHandle);
				glBindVertexArray(meshData->VAOHandle);

				// Determine index type and size
				bool indices16Bit;
				std::vector<uint16> Indices16BitVector;

				if (modelMesh.IndexCount <= 0xffff)
				{
					meshData->GlIndexType = GL_UNSIGNED_SHORT;
					indices16Bit = true;
				} 
				else
				{
					meshData->GlIndexType = GL_UNSIGNED_INT;
					indices16Bit = false;
				}

				// Copy data
				for (uint32 index = 0, l = modelMesh.IndexCount; index < l; index++)
				{
					uint32 vertIndex = modelMesh.Indices[index];
					Indices16BitVector.push_back(static_cast<uint16>(vertIndex));
				}

				// Generate index buffer
				glGenBuffers(1, &meshData->IndexBufferHandle);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->IndexBufferHandle);

				if (indices16Bit)
				{
					Indices16BitVector.shrink_to_fit();
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelMesh.IndexCount * sizeof(uint16), &Indices16BitVector[0], GL_STATIC_DRAW);
				}
				else
				{
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelMesh.IndexCount * sizeof(uint32), modelMesh.Indices, GL_STATIC_DRAW);
				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				// Generate Vertex buffer
				glGenBuffers(1, &meshData->VertexBufferHandle);
				glBindBuffer(GL_ARRAY_BUFFER, meshData->VertexBufferHandle);
				
				// Size = VertexCount * number of floats per vertex * sizeof(float)
				glBufferData(GL_ARRAY_BUFFER, modelMesh.VertexCount * modelMesh.VertexStride * sizeof(float), modelMesh.Vertices, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glBindVertexArray(0);

				meshes.push_back(meshData);
			}

			MeshLoadData* result = new MeshLoadData();
			result->Meshes = meshes;
			return result;
		}
	};
};

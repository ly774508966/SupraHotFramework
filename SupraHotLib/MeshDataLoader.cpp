#include "MeshDataLoader.h"
#include <unordered_map>
#include "Material.h"
#include "FileReader.h"
#include "StringUtil.h"
#include "MaterialLoader.h"
#include "Face.h"
#include "Vertex.h"
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
				Material* material = meshLoadData->MaterialsMap[meshLoadData->MaterialIDs[m]];
				material->SetShader(ShaderLibrary::GetInstance()->SelectShaderForMaterialAndMeshData(meshData, material));
				meshComponents.push_back(new MeshComponent(meshData, material));
			}

			delete meshLoadData;
			return meshComponents;
		}

		MeshLoadData* MeshDataLoader::LoadSHFM(std::string path)
		{
			SHFModelFile model = SHFMBinaryLoader::GetInstance().LoadFromFile(path);

			// Create map for materials
			std::unordered_map<uint32, Graphics::Material*> materialsMap;
			std::vector<MeshData*> meshes {};
			std::vector<uint32> materialIDs{};

			for (uint32 i = 0; i < model.MeshCount; i++)
			{
				SHFModel::Mesh& modelMesh = model.Meshes[i];
				SHFModel::Material& modelMaterial = model.Materials[modelMesh.MaterialID];
				
				// Load material, if it wasn't loaded before.
				if (materialsMap.find(modelMesh.MaterialID) == materialsMap.end())
				{
					#if DEVELOPMENT == 1
						SHF_PRINTF("Generating material %s \n", modelMaterial.Name.c_str());
					#endif

					// Set values
					Graphics::Material* material = new Graphics::Material();
					material->ID = modelMaterial.ID;
					material->Name = modelMaterial.Name;
					material->Ka = modelMaterial.Ka; 
					material->Kd = modelMaterial.Kd; 
					material->Ks = modelMaterial.Ks; 
					material->Ke = modelMaterial.Ke; 
					material->Ns = modelMaterial.Ns; 
					material->Roughness = modelMaterial.Roughness;
					material->Metalness = modelMaterial.Metalness;
					material->F0 = modelMaterial.F0;

					// Load textures.
					if (modelMaterial.AlbeoMapPathLength > 0)
					{
						// Load albedo map
						Texture2D* texture = nullptr;

						if (FileSystem::GetInstance()->FileExists("", modelMaterial.AlbedoMapPath))
						{
							texture = new Texture2D();
							texture->Load(modelMaterial.AlbedoMapPath);
						}
						else if (FileSystem::GetInstance()->FileExists("", Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.AlbedoMapPath))
						{
							modelMaterial.AlbedoMapPath = Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.AlbedoMapPath;
							texture = new Texture2D();
							texture->Load(modelMaterial.AlbedoMapPath);
						} 
						
#if DEVELOPMENT == 1
						SHF_PRINTF("directory = %s \n", Utils::StringUtil::GetPathFromFilePath(path).c_str());
#endif
						material->SetAlbedoMap(texture);
					}

					if (modelMaterial.NormalMapPathLength > 0)
					{
						// Load normal map
						Texture2D* texture = nullptr;

						if (FileSystem::GetInstance()->FileExists("", modelMaterial.NormalMapPath))
						{
							texture = new Texture2D();
							texture->Load(modelMaterial.NormalMapPath);
						}
						else if (FileSystem::GetInstance()->FileExists("", Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.NormalMapPath))
						{
							modelMaterial.NormalMapPath = Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.NormalMapPath;
							texture = new Texture2D();
							texture->Load(modelMaterial.NormalMapPath);
						}

						material->SetNormalMap(texture);
					}

					if (modelMaterial.SpecularMapPathLength > 0)
					{
						// Load roughness map
						Texture2D* texture = nullptr;

						if (FileSystem::GetInstance()->FileExists("", modelMaterial.SpecularMapPath))
						{
							texture = new Texture2D();
							texture->Load(modelMaterial.SpecularMapPath);
						}
						else if (FileSystem::GetInstance()->FileExists("", Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.SpecularMapPath))
						{
							modelMaterial.SpecularMapPath = Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.SpecularMapPath;
							texture = new Texture2D();
							texture->Load(modelMaterial.SpecularMapPath);
						}

						material->SetRoughnessMap(texture);
					}

					if (modelMaterial.ShininessReflectionMapPathLength > 0)
					{
						// Load metalness map
						Texture2D* texture = nullptr;

						if (FileSystem::GetInstance()->FileExists("", modelMaterial.ShininessReflectionMapPath))
						{
							texture = new Texture2D();
							texture->Load(modelMaterial.ShininessReflectionMapPath);
						}
						else if (FileSystem::GetInstance()->FileExists("", Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.ShininessReflectionMapPath))
						{
							modelMaterial.ShininessReflectionMapPath = Utils::StringUtil::GetPathFromFilePath(path) + modelMaterial.ShininessReflectionMapPath;
							texture = new Texture2D();
							texture->Load(modelMaterial.ShininessReflectionMapPath);
						}

						material->SetMetalnessMap(texture);
					}

					// Todo: We also need to create VMF roughness maps from metalness & roughness

					if (modelMaterial.OpacityMapPathLength > 0)
					{
						// Load alpha map

						// If we have an alpha mask, we can combine it with the albedo map!
						// There is currently no need to waste memory for another r8-texture

						//Texture2D* texture = new Texture2D();
						//texture->Load(modelMaterial.OpacityMapPath);
						//material->SetAlphamap(texture);
					}

					// Push material into hashmap
					materialsMap[modelMesh.MaterialID] = material; 
				}
			}

			for (uint32 i = 0; i < model.MeshCount; i++)
			{
				SHFModel::Mesh& modelMesh = model.Meshes[i];
				MeshData* meshData = new MeshData();

				meshData->Name = modelMesh.Name;
				meshData->FaceCount = modelMesh.FaceCount;
				meshData->VertexCount = modelMesh.VertexCount;

				if (modelMesh.VertexAttributes == SHFModel::VertexBitfield::POSITION)
				{
					meshData->HasPositionData = true;
				}
				else if (modelMesh.VertexAttributes == SHFModel::VertexBitfield::POSITION_NORMAL)
				{
					meshData->HasPositionData = true;
					meshData->HasNormalData = true;
				}
				else if (modelMesh.VertexAttributes == SHFModel::VertexBitfield::POSITION_NORMAL_UV)
				{
					meshData->HasPositionData = true;
					meshData->HasNormalData = true;
					meshData->HasUVData = true;
				}
				else if (modelMesh.VertexAttributes == SHFModel::VertexBitfield::POSITION_NORMAL_UV_TANGENT_BITANGENT)
				{
					meshData->HasPositionData = true;
					meshData->HasNormalData = true;
					meshData->HasUVData = true;
					meshData->HasBiTangentData = true;
					meshData->HasTangentData = true;
				}

				// Generate vao
				glGenVertexArrays(1, &meshData->VAOHandle);
				glBindVertexArray(meshData->VAOHandle);

				// Determine index type and size
				bool indices16Bit = true;
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

				// Push back the material id!
				materialIDs.push_back(modelMesh.MaterialID);
			}

			MeshLoadData* result = new MeshLoadData();
			result->Meshes = meshes;
			result->MaterialsMap = materialsMap;
			result->MaterialIDs = materialIDs;

			return result;
		}
	};
};

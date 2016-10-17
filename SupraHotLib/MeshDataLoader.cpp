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

		std::vector<MeshData*> MeshDataLoader::Load(std::string pathToSHFM)
		{
			return LoadSHFM(pathToSHFM);
		}

		std::vector<MeshData*> MeshDataLoader::LoadOBJX(std::string path)
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("Loading Mesh: %s \n", path.c_str());
#endif

			/* container data */
			uint32 meshcount = 0;
			uint32 vertexcount = 0;
			uint32 normalcount = 0;
			uint32 tangentcount = 0;
			uint32 uvcount = 0;

			MeshData* tempMeshData = new MeshData();
			bool gotMesh = false;
			std::unordered_map<std::string, Material>  materials;
			std::vector<MeshData*> data;
			std::string pathToObjx = "";

			/* reader variables */
			FileReader* fileReader = FileReader::GetInstance();
			std::vector<std::string> fileContent = fileReader->ReadFile(path);

			if (fileContent.size() == 0)
			{
				return data;
			}

			/* interpret model header */
			for (unsigned int i = 0; i < fileContent.size(); i++)
			{
				std::string line = fileContent[i];

				if (line.find("op") == 0)
				{
					pathToObjx = StringUtil::trim(StringUtil::Split(line, "op", false).at(0)).c_str();
				}
				else if (line.find("mp") == 0)
				{
					/* pre-load material file */
					materials = MaterialLoader::GetInstance()->LoadMTLFile(StringUtil::trim(StringUtil::Split(line, "mp").at(1)).c_str());
				}
				else if (line.find("m ") == 0)
				{
					SHF_SSCANF(line.c_str(), "m %u", &meshcount);
				}
				else if (line.find("vc") == 0)
				{
					SHF_SSCANF(line.c_str(), "vc %u", &vertexcount);
				}
				else if (line.find("nc") == 0)
				{
					SHF_SSCANF(line.c_str(), "nc %u", &normalcount);
				}
				else if (line.find("tc") == 0)
				{
					SHF_SSCANF(line.c_str(), "tc %u", &tangentcount);
				}
				else if (line.find("uc") == 0)
				{
					SHF_SSCANF(line.c_str(), "uc %u", &uvcount);
				}
				else if (line.find("o ") == 0)
				{
					if (gotMesh)
					{
						data.push_back(tempMeshData);
						Vec3 origin = tempMeshData->Origin;
#if DEVELOPMENT == 1
						SHF_PRINTF("Pushing Mesh %s with %d faces at Origin [%f %f %f]\n", tempMeshData->Name.c_str(), tempMeshData->FaceCount, origin.x, origin.y, origin.z);
#endif
					}

					gotMesh = true;
					tempMeshData = new MeshData();
					std::string tempName = StringUtil::Split(line, "o ", false).at(0);
					tempName = StringUtil::trim(tempName);
					tempMeshData->Name = tempName.c_str();
				}
				else if (line.find("mtl") == 0)
				{
					tempMeshData->MeshMaterial = materials[StringUtil::trim(StringUtil::Split(line, "mtl").at(1)).c_str()];
				}
				else if (line.find("wo") == 0)
				{
					float x, y, z;
					SHF_SSCANF(line.c_str(), "wo %f %f %f", &x, &y, &z);
					tempMeshData->Origin = Vec3(x, y, z);
				}
				else if (line.find("f ") == 0)
				{
					unsigned int f;
					SHF_SSCANF(line.c_str(), "f %u", &f);
					tempMeshData->FaceCount = f;
				}
			}


			if (gotMesh)
			{
				data.push_back(tempMeshData);
				Vec3 origin = tempMeshData->Origin;
#if DEVELOPMENT == 1
				SHF_PRINTF("Pushing Mesh %s with %d faces at Origin [%f %f %f]\n", tempMeshData->Name.c_str(), tempMeshData->FaceCount, origin.x, origin.y, origin.z);
#endif
			}


#if DEVELOPMENT == 1
			SHF_PRINTF("Loading OBJX: %s \n", pathToObjx.c_str());
#endif

			/* open binary file */
			
			FILE *vertex_file = FileSystem::GetInstance()->GetFile("", pathToObjx.c_str(), "rb");
		
			if (vertex_file == nullptr)
			{
				SHF_PRINTF("Can't open pathToObjx %s \n", pathToObjx.c_str());
				return data;
			}

			/* read binary file */
			for (uint32 i = 0; i < meshcount; i++)
			{
				for (uint32 j = 0, l = data[i]->FaceCount; j < l; j++)
				{
					Vertex v0;
					Vertex v1;
					Vertex v2;

					Face face;

					/* read in floats */
					float binary[27];
					fread(&binary, 27, sizeof(float), vertex_file);

					v0.SetPosition(Vec3(binary[0], binary[1], binary[2]));
					v0.SetUVCoord(Vec2(binary[3], binary[4]));
					v0.SetNormal(Vec3(binary[5], binary[6], binary[7]));
					v0.SetTangent(Vec3(binary[24], binary[25], binary[26]));
					v0.SetBiNormal(v0.GetNormal().cross(v0.GetTangent()));

					v1.SetPosition(Vec3(binary[8], binary[9], binary[10]));
					v1.SetUVCoord(Vec2(binary[11], binary[12]));
					v1.SetNormal(Vec3(binary[13], binary[14], binary[15]));
					v1.SetTangent(Vec3(binary[24], binary[25], binary[26]));
					v1.SetBiNormal(v1.GetNormal().cross(v1.GetTangent()));

					v2.SetPosition(Vec3(binary[16], binary[17], binary[18]));
					v2.SetUVCoord(Vec2(binary[19], binary[20]));
					v2.SetNormal(Vec3(binary[21], binary[22], binary[23]));
					v2.SetTangent(Vec3(binary[24], binary[25], binary[26]));
					v2.SetBiNormal(v2.GetNormal().cross(v2.GetTangent()));

					/* add vertices to a temp face and push that face back to the mesh */
					face.SetVertex(v0, 0);
					face.SetVertex(v1, 1);
					face.SetVertex(v2, 2);

					data[i]->Faces.push_back(face);
				}

				data[i]->HasPositionData = true;
				data[i]->HasNormalData = true;
				data[i]->HasUVData = true;
				data[i]->HasTangentData = true;
				data[i]->HasBiNormalData = true;
			
				// Generate GL Buffer Handles
				{
					MeshData* currentMeshData = data[i];
					/* generate and bind vertex array object */
#if DEVELOPMENT == 1
					SHF_PRINTF("Finishing MeshGL: %s (Material: %s)\n", currentMeshData->Name.c_str(), currentMeshData->MeshMaterial.GetName().c_str());
#endif
					glGenVertexArrays(1, &currentMeshData->VAOId);
					glBindVertexArray(currentMeshData->VAOId);

					/* genereate vertex buffers */
					if (currentMeshData->HasIndexData)
					{
						glGenBuffers(1, &currentMeshData->IndexBufferHandle);
					}

					if (currentMeshData->HasPositionData)
					{
						glGenBuffers(1, &currentMeshData->PositionBufferHandle);
					}

					if (currentMeshData->HasNormalData)
					{
						glGenBuffers(1, &currentMeshData->NormalBufferHandle);
					}

					if (currentMeshData->HasUVData)
					{
						glGenBuffers(1, &currentMeshData->UVBufferHandle);
					}

					if (currentMeshData->HasTangentData)
					{
						glGenBuffers(1, &currentMeshData->TangentBufferHandle);
					}

					if (currentMeshData->HasBiNormalData)
					{
						glGenBuffers(1, &currentMeshData->BiNormalBufferHandle);
					}

					std::vector<float> positions;
					std::vector<float> normals;
					std::vector<float> tangents;
					std::vector<float> binormals;
					std::vector<float> uvcoords;

					for (uint32 j = 0, l = static_cast<uint32>(currentMeshData->Faces.size()); j < l; j++)
					{
						positions.push_back(currentMeshData->Faces[j].GetVertex(0).GetPosition().x);
						positions.push_back(currentMeshData->Faces[j].GetVertex(0).GetPosition().y);
						positions.push_back(currentMeshData->Faces[j].GetVertex(0).GetPosition().z);

						positions.push_back(currentMeshData->Faces[j].GetVertex(1).GetPosition().x);
						positions.push_back(currentMeshData->Faces[j].GetVertex(1).GetPosition().y);
						positions.push_back(currentMeshData->Faces[j].GetVertex(1).GetPosition().z);

						positions.push_back(currentMeshData->Faces[j].GetVertex(2).GetPosition().x);
						positions.push_back(currentMeshData->Faces[j].GetVertex(2).GetPosition().y);
						positions.push_back(currentMeshData->Faces[j].GetVertex(2).GetPosition().z);

						normals.push_back(currentMeshData->Faces[j].GetVertex(0).GetNormal().x);
						normals.push_back(currentMeshData->Faces[j].GetVertex(0).GetNormal().y);
						normals.push_back(currentMeshData->Faces[j].GetVertex(0).GetNormal().z);

						normals.push_back(currentMeshData->Faces[j].GetVertex(1).GetNormal().x);
						normals.push_back(currentMeshData->Faces[j].GetVertex(1).GetNormal().y);
						normals.push_back(currentMeshData->Faces[j].GetVertex(1).GetNormal().z);

						normals.push_back(currentMeshData->Faces[j].GetVertex(2).GetNormal().x);
						normals.push_back(currentMeshData->Faces[j].GetVertex(2).GetNormal().y);
						normals.push_back(currentMeshData->Faces[j].GetVertex(2).GetNormal().z);

						tangents.push_back(currentMeshData->Faces[j].GetVertex(0).GetTangent().x);
						tangents.push_back(currentMeshData->Faces[j].GetVertex(0).GetTangent().y);
						tangents.push_back(currentMeshData->Faces[j].GetVertex(0).GetTangent().z);

						tangents.push_back(currentMeshData->Faces[j].GetVertex(1).GetTangent().x);
						tangents.push_back(currentMeshData->Faces[j].GetVertex(1).GetTangent().y);
						tangents.push_back(currentMeshData->Faces[j].GetVertex(1).GetTangent().z);

						tangents.push_back(currentMeshData->Faces[j].GetVertex(2).GetTangent().x);
						tangents.push_back(currentMeshData->Faces[j].GetVertex(2).GetTangent().y);
						tangents.push_back(currentMeshData->Faces[j].GetVertex(2).GetTangent().z);

						binormals.push_back(currentMeshData->Faces[j].GetVertex(0).GetBiNormal().x);
						binormals.push_back(currentMeshData->Faces[j].GetVertex(0).GetBiNormal().y);
						binormals.push_back(currentMeshData->Faces[j].GetVertex(0).GetBiNormal().z);

						binormals.push_back(currentMeshData->Faces[j].GetVertex(1).GetBiNormal().x);
						binormals.push_back(currentMeshData->Faces[j].GetVertex(1).GetBiNormal().y);
						binormals.push_back(currentMeshData->Faces[j].GetVertex(1).GetBiNormal().z);

						binormals.push_back(currentMeshData->Faces[j].GetVertex(2).GetBiNormal().x);
						binormals.push_back(currentMeshData->Faces[j].GetVertex(2).GetBiNormal().y);
						binormals.push_back(currentMeshData->Faces[j].GetVertex(2).GetBiNormal().z);

						uvcoords.push_back(currentMeshData->Faces[j].GetVertex(0).GetUVCoord().x);
						uvcoords.push_back(currentMeshData->Faces[j].GetVertex(0).GetUVCoord().y);

						uvcoords.push_back(currentMeshData->Faces[j].GetVertex(1).GetUVCoord().x);
						uvcoords.push_back(currentMeshData->Faces[j].GetVertex(1).GetUVCoord().y);

						uvcoords.push_back(currentMeshData->Faces[j].GetVertex(2).GetUVCoord().x);
						uvcoords.push_back(currentMeshData->Faces[j].GetVertex(2).GetUVCoord().y);
					}

					glBindBuffer(GL_ARRAY_BUFFER, currentMeshData->PositionBufferHandle);
					glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);

					glBindBuffer(GL_ARRAY_BUFFER, currentMeshData->NormalBufferHandle);
					glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

					glBindBuffer(GL_ARRAY_BUFFER, currentMeshData->TangentBufferHandle);
					glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), &tangents[0], GL_STATIC_DRAW);

					glBindBuffer(GL_ARRAY_BUFFER, currentMeshData->BiNormalBufferHandle);
					glBufferData(GL_ARRAY_BUFFER, binormals.size() * sizeof(float), &binormals[0], GL_STATIC_DRAW);

					glBindBuffer(GL_ARRAY_BUFFER, currentMeshData->UVBufferHandle);
					glBufferData(GL_ARRAY_BUFFER, uvcoords.size() * sizeof(float), &uvcoords[0], GL_STATIC_DRAW);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);

#if DEVELOPMENT == 1
					int err = glGetError();
					if (err != 0)
					{
						SHF_PRINTF("Error %d happened while creating mesh data gl buffer handles \n", err);
					}
#endif
				}


#if DEVELOPMENT == 1
				SHF_PRINTF("Finished Mesh: %s \n", data[i]->Name.c_str());
#endif

				data[i]->PositionData.clear();
				data[i]->NormalData.clear();
				data[i]->UVData.clear();
				data[i]->TangentData.clear();
				data[i]->BiNormalData.clear();
				data[i]->IndexData.clear();

			}

			fclose(vertex_file);

#if DEVELOPMENT == 1
			SHF_PRINTF("Procssed Meshs: %u with Vertexcount: %d \n", static_cast<uint32>(data.size()), vertexcount);
#endif
			return data;
		}

		std::vector<MeshData*> MeshDataLoader::LoadSHFM(std::string path)
		{
			SHFModelFile model = SHFMBinaryLoader::GetInstance().LoadFromFile(path);


			// Create map for materials
			std::unordered_map<uint32, Graphics::Material*> materialsMap;


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
					}

					if (modelMaterial.NormalMapPathLength > 0)
					{
						// Load normal map
					}

					if (modelMaterial.SpecularMapPathLength > 0)
					{
						// Load roughness map
					}

					if (modelMaterial.ShininessReflectionMapPathLength > 0)
					{
						// Load metalness map
					}

					// Todo: We also need to create VMF roughness maps from metalness & roughness

					if (modelMaterial.OpacityMapPathLength > 0)
					{
						// Load alpha map

						// If we have an alpha mask, we can combine it with the albedo map!
						// There is currently no need to waste memory for another r8-texture
					}

					// Push material into hashmap
					materialsMap[modelMesh.MaterialID] = material; 
					// std::make_pair(key,value)
				}
			}


			for (uint32 i = 0; i < model.MaterialCount; i++)
			{
				SHFModel::Material& material = model.Materials[i];
				
				// Interpret data here.

				// Load materials when needed.
			}

			return std::vector<MeshData*>();
		}
	};
};

#include "ShaderLibrary.h"
#include <iostream>
#include "Utility.h"
#include "MeshData.h"
#include <map>
#include "ShaderParser.h"

namespace SupraHot
{
	namespace Graphics
	{
		ShaderLibrary::ShaderLibrary()
		{
		}

		ShaderLibrary::~ShaderLibrary()
		{
		}

		Shader* ShaderLibrary::SelectShaderForMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::Material* material)
		{
			uint64 shaderIndex = 0;

			if (meshData->HasNormalData) 
			{
				shaderIndex |= uint64(MeshVertexShader::Normal);
			}

			if (meshData->HasUVData)
			{
				shaderIndex |= uint64(MeshVertexShader::UV);
			}

			if (meshData->HasTangentData || meshData->HasBiTangentData)
			{
				shaderIndex |= uint64(MeshVertexShader::TangentBiTangent);
			}

			if (material->GetAlbedoMap() != nullptr)
			{
				shaderIndex |= uint64(MeshPixelShader::AlbedoMap);
			}

			if (material->GetNormalMap() != nullptr)
			{
				shaderIndex |= uint64(MeshPixelShader::NormalMap);
			}

			if (material->GetRoughnessMap() != nullptr)
			{
				shaderIndex |= uint64(MeshPixelShader::RoughnessMap);
			}

			if (material->GetMetalnessMap() != nullptr)
			{
				shaderIndex |= uint64(MeshPixelShader::MetalnessMap);
			}

			if (material->GetSpecularMap() != nullptr)
			{
				shaderIndex |= uint64(MeshPixelShader::SpecularMap);
			}

			if (material->GetComboMap() != nullptr)
			{
				shaderIndex |= uint64(MeshPixelShader::ComboMap);
			}
#if DEVELOPMENT == 1
			SHF_PRINTF("Selected %s for Mesh : %s \n", MeshShaders[shaderIndex]->GetName().c_str(), meshData->Name.c_str());
#endif
			return MeshShaders[shaderIndex];
		}

		void ShaderLibrary::Initialize()
		{
			std::string directoryPath = "Shaders/GL/";

#ifdef PLATFORM_ANDROID
			directoryPath = "Shaders/GLES3/";
#endif


			// Init skybox shaders
			{
				Shader* skyboxCubeShader = new Shader();
				skyboxCubeShader->SetName("Skybox Cubemap Shader");
				skyboxCubeShader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "skybox.vs.glsl");
				skyboxCubeShader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "skybox.fs.glsl");
				skyboxCubeShader->CompileShader();
				Skybox[uint32(SkyboxShader::CubeMap)] = skyboxCubeShader;


				Shader* skyboxSphereShader = new Shader();
				skyboxSphereShader->SetName("Skybox Sphere Shader");
				skyboxSphereShader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "skybox.vs.glsl");
				skyboxSphereShader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "skybox-sphere.fs.glsl");
				skyboxSphereShader->CompileShader();
				Skybox[uint32(SkyboxShader::SphereMap)] = skyboxSphereShader;
			}

			// Screen space shaders
			{
				Shader* fboShader = new Shader();
				fboShader->SetName("Render to screen");
				fboShader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "fbo.vs.glsl");
				fboShader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "fbo.fs.glsl");
				fboShader->CompileShader();
				ScreenSpace[uint32(ScreenSpace::RenderToScreen)] = fboShader;

			}

			{
				ShaderCompileOptions opts;

				uint32 shaderInputVertexAttribs = 3;
				uint32 shaderInputTextures = 6;
				
				for (uint32 v = 0, vl = static_cast<uint32>(pow(2, shaderInputVertexAttribs)); v < vl; v++)
				{
					
					std::vector<bool> vertexAttribBools = Utils::Utility::GetBoolCombinations(v, shaderInputVertexAttribs);

					bool hasNormals = vertexAttribBools[0];
					bool hasUV = vertexAttribBools[1];
					bool hasTangentBiTangent = vertexAttribBools[2];

					// if we have tangents but no normals, we can skip this shader
					if (hasTangentBiTangent && !hasNormals)
					{
						continue;
					}
	
					for (uint32 t = 0, tl = static_cast<uint32>(pow(2, shaderInputTextures)); t < tl; t++)
					{
						bool compileShader = true;

						opts.Define("_Normals", hasNormals);
						opts.Define("_UV", hasUV);
						opts.Define("_TangentsBiTangents", hasTangentBiTangent);

						std::vector<bool> textureBools = Utils::Utility::GetBoolCombinations(t, shaderInputTextures);

						bool hasAlbedoMap = textureBools[0];
						bool hasNormalMap = textureBools[1];
						bool hasRoughnessMap = textureBools[2];
						bool hasMetalnessMap = textureBools[3];
						bool hasSpecularMap = textureBools[4];
						bool hasComboMap = textureBools[5];

						opts.Define("_AlbedoMap", hasAlbedoMap);
						opts.Define("_NormalMap", hasNormalMap);
						opts.Define("_RoughnessMap", hasRoughnessMap);
						opts.Define("_MetalnessMap", hasMetalnessMap);
						opts.Define("_SpecularMap", hasSpecularMap);
						opts.Define("_ComboMap", hasComboMap);

						// if we run into problems, we can comment this section out and load ALL possible permutations
						// an then just skip the ones, which aren't compiling. LUL.

						if (hasNormalMap && !hasNormals) // NormalMap set to true && Normals set to false
						{
							compileShader = false;
						}
						else if (hasNormalMap && !hasTangentBiTangent) // NormalMap set to true && Tangents set to false
						{
							compileShader = false;
						}

						if (!hasUV) // no UV's
						{
							if (hasAlbedoMap || hasNormalMap || hasRoughnessMap
								|| hasMetalnessMap || hasSpecularMap || hasComboMap)
							{
								compileShader = false;
							}

						}

						if (compileShader)
						{
							//Generate shader index
							std::string shaderName = "";

							uint64 shaderIndex = 0;
							if (hasNormals)
							{
								shaderIndex |= uint64(MeshVertexShader::Normal);
								shaderName += "Nrml | ";
							}

							if (hasUV)
							{
								shaderIndex |= uint64(MeshVertexShader::UV);
								shaderName += "UV | ";
							}

							if (hasTangentBiTangent)
							{
								shaderIndex |= uint64(MeshVertexShader::TangentBiTangent);
								shaderName += "TanBiTan | ";
							}

							if (hasAlbedoMap)
							{
								shaderIndex |= uint64(MeshPixelShader::AlbedoMap);
								shaderName += "Albedo | ";
							}

							if (hasNormalMap)
							{
								shaderIndex |= uint64(MeshPixelShader::NormalMap);
								shaderName += "NrmlM | ";
							}

							if (hasRoughnessMap)
							{
								shaderIndex |= uint64(MeshPixelShader::RoughnessMap);
								shaderName += "Rgh | ";
							}

							if (hasMetalnessMap)
							{
								shaderIndex |= uint64(MeshPixelShader::MetalnessMap);
								shaderName += "Mtl | ";
							}

							if (hasSpecularMap)
							{
								shaderIndex |= uint64(MeshPixelShader::SpecularMap);
								shaderName += "Spec | ";
							}

							if (hasComboMap)
							{
								shaderIndex |= uint64(MeshPixelShader::ComboMap);
								shaderName += "Cmb | ";
							}

							// Load shader with compile options.
							Shader* shader = new Shader();
							shader->SetName("Mesh: #" + std::to_string(shaderIndex) + " (" + shaderName + ")");
							shader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
							shader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
							shader->CompileShader();
							MeshShaders[shaderIndex] = shader;
						}

						opts.Reset();
					}
				}

				SHF_PRINTF("Created %llu shader permutations \n", MeshShaders.size());
			}


			// - - - - - - - - - - - - - - - - 
			// Init shader descriptions
			// - - - - - - - - - - - - - - - - 

			std::string baseDirectoryPath = "Shaders/Description/";

			// use dirent.h here to iterate over all files inside this directory
			std::vector<std::string> shaderDescriptions = {"MeshBasicShader.json"};

			for (size_t i = 0, l = shaderDescriptions.size(); i < l; ++i)
			{
				ShaderParser::ShaderDescription* shaderDescription = ShaderParser::GetInstance()->Parse(baseDirectoryPath + shaderDescriptions[i]);
				
				if (shaderDescription != nullptr)
				{
					
					std::string name = shaderDescription->Name;
					std::string description = shaderDescription->Description;

					std::string vertexShaderPath = shaderDescription->VertexShaderPath;
					std::string pixelShaderPath = shaderDescription->PixelShaderPath;

					std::unordered_map<std::string, std::vector<std::string>>* definedWhen = &shaderDescription->DefinedWhen;
					std::unordered_map<std::string, std::vector<std::string>>* dependencies = &shaderDescription->Dependencies;
					std::unordered_map<std::string, std::string>* uniforms = &shaderDescription->Uniforms;


					// Create indexed versions of DefinedWhen
					std::vector<std::string> indexedDefinedWhen;
					{
						typedef std::unordered_map<std::string, std::vector<std::string>>::iterator it_type;
						for (it_type iterator = definedWhen->begin(); iterator != definedWhen->end(); ++iterator)
						{
							indexedDefinedWhen.push_back(iterator->first);
						}
					}

					// Add internals to indexedDefines at the end.
					indexedDefinedWhen.push_back("_Normals");
					indexedDefinedWhen.push_back("_UV");
					indexedDefinedWhen.push_back("_TangentsBiTangents");

					SHF_PRINTF("Create Shader permutations for %s \n", name.c_str());
					SHF_PRINTF("[ %llu Uniforms, %llu DefinedWhen, %llu Dependencies ]\n", uniforms->size(), definedWhen->size(), dependencies->size());

					// Prepare boolean combinations for 'DefinedWhen'-Values

					uint32 definedWhenCount = static_cast<uint32>(indexedDefinedWhen.size());
					for (uint32 u = 0, ul = static_cast<uint32>(pow(2, definedWhenCount)); u < ul; ++u)
					{
						bool compileShader = true;
						std::vector<bool> definedWhenBooleans = Utils::Utility::GetBoolCombinations(u, definedWhenCount);
						std::unordered_map<std::string, bool> definedWhenOptions;
						ShaderCompileOptions compileOptions;

						// Build up map first.
						for (uint32 definedIdx = 0; definedIdx < definedWhenBooleans.size(); ++definedIdx)
						{
							std::string defineName = indexedDefinedWhen[definedIdx];
							definedWhenOptions[defineName] = definedWhenBooleans[definedIdx];
						}

						// Check dependencies
						// if we don't meet the dependencies for this current define, we need to break
						typedef std::unordered_map<std::string, bool>::iterator it_type;
						for (it_type iterator = definedWhenOptions.begin(); iterator != definedWhenOptions.end(); ++iterator)
						{
							std::string defineName = iterator->first;
							compileOptions.Define(iterator->first, iterator->second);

							if (iterator->second) // check for deps only if the the define is set to true.....
							{
								if (dependencies->find(defineName) != dependencies->end())
								{
									std::vector<std::string>& deps = dependencies->at(defineName);

									for (uint32 depIdx = 0; depIdx < deps.size(); ++depIdx)
									{
										if (definedWhenOptions.find(deps[depIdx]) != definedWhenOptions.end()
											&& definedWhenOptions[deps[depIdx]] == false
											)
										{
											compileShader = false;
											break;
										}
									}
								}
							}

							if (!compileShader)
							{
								break;
							}
							
						}


						if (compileShader)
						{
							// Create MaterialProperties from Uniforms.
							// shaderDescription->Uniforms;

							//compileOptions.Print();

							// Create ShaderCompileOptions

							// Inject them into the source code

							// Compile shader

							// Create somesort of bitfield based on the properties.
						}

					}
				}
			}
		}

		void ShaderLibrary::Destroy()
		{
			for (uint32 i = 0, l = uint32(SkyboxShader::Count); i < l; ++i)
			{
				Shader* shader = Skybox[i];
				if (shader != nullptr)
				{
					shader->Destroy();
					delete shader;
				}
			}

			for (uint32 i = 0, l = uint32(ScreenSpace::Count); i < l; ++i)
			{
				Shader* shader = ScreenSpace[i];
				if (shader != nullptr)
				{
					shader->Destroy();
					delete shader;
				}
			}

			typedef std::unordered_map<uint64, Shader*>::iterator it_type;
			for (it_type iterator = MeshShaders.begin(); iterator != MeshShaders.end(); ++iterator) {
				Shader* shader = iterator->second;
				shader->Destroy();
				delete shader;
			}

		}

		ShaderLibrary* ShaderLibrary::GetInstance()
		{
			static ShaderLibrary* instance(new ShaderLibrary);
			return instance;
		}

	};
};
#include <map>
#include "ShaderLibrary.h"
#include "ShaderDescription.h"
#include "Utility.h"
#include "MeshData.h"
#include "ShaderParser.h"
#include "ShaderMaterial.h"

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

		Shader* ShaderLibrary::SelectShaderForShaderMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::ShaderMaterial* material)
		{
			// first get description
			ShaderDescription* description = material->GetShaderDescription();
			auto shadersMap = GetShaders(description);

			// generate shader index
			uint64 shaderIndex = 0;
			std::vector<MaterialProperty*>* materialProperties = material->GetMaterialProperties();
			std::vector<std::string> alreadyProcessedProperties;

			for (size_t i = 0, l = materialProperties->size(); i < l; ++i)
			{
				MaterialProperty* materialProperty = materialProperties->at(i);
				std::string name = materialProperty->GetName();

				// Check if there is a define for this property in the BitField
				// Note: Add "_" in front of the name, since properties can not have the same name as defines!
				auto it = description->BitShiftedIndices.find("_" + name);

				// We found something. 
				// Define this property by it self.
				if (it != description->BitShiftedIndices.end())
				{
					uint64 propertyBitfieldIndex = description->BitShiftedIndices["_" + name];
					shaderIndex |= propertyBitfieldIndex;

					SHF_PRINTF("%s defined %s \n", name.c_str(), ("_" + name).c_str());
				}

				alreadyProcessedProperties.push_back(name);
			}

			// these values are atm hardcoded
			// TODO: store these somewhere globally available, 
			// so that we can change them at one convinient place in the future

			if (meshData->HasNormalData)
			{
				// _Normals
				uint64 propertyBitfieldIndex = description->BitShiftedIndices["_Normals"];
				shaderIndex |= propertyBitfieldIndex;
			}

			if (meshData->HasUVData)
			{
				// _UV
				uint64 propertyBitfieldIndex = description->BitShiftedIndices["_UV"];
				shaderIndex |= propertyBitfieldIndex;
			}

			if (meshData->HasTangentData | meshData->HasBiTangentData)
			{
				// _TangentsBiTangents
				uint64 propertyBitfieldIndex = description->BitShiftedIndices["_TangentsBiTangents"];
				shaderIndex |= propertyBitfieldIndex;
			}


			// Now resolve definedWhen & dependencies

			{
				// Now check the definedWhen-Map
				// For this we need to use the property's real name and loop through the dependencies maps
				typedef std::unordered_map<std::string, std::vector<std::string>>::iterator DefinedWhenIterator;
				for (DefinedWhenIterator defIt = description->DefinedWhen.begin(); defIt != description->DefinedWhen.end(); ++defIt)
				{
					// Check if this define has dependencies on another definedWhen-attribute & if we can meet them
					std::string defineName = defIt->first;
					std::vector<std::string>* defineArray = &(defIt->second);
					bool isDefined = true;

					for (size_t i = 0, l = defineArray->size(); i < l; ++i)
					{
						std::string defineEntry = defineArray->at(i);
						isDefined = ResolveDefinedWhen(defineEntry, description, shaderIndex, &alreadyProcessedProperties);
					}

					if (isDefined)
					{
						SHF_PRINTF("Defined %s \n", defineName.c_str());

						// Check for deps.

						uint64 propertyBitfieldIndex = description->BitShiftedIndices[defineName];
						shaderIndex |= propertyBitfieldIndex;
					}
				}
			}


			return shadersMap->at(shaderIndex);
		}

		bool ShaderLibrary::ResolveDefinedWhen(std::string defineEntry, ShaderDescription* description, uint64 shaderIndex, std::vector<std::string>* alreadyProcessedProperties)
		{
			// if entry begins with "_", we need to check for defines
			if (defineEntry.find("_") == 0)
			{
				// check for deps. 
				// Dependency is already met
				uint64 bitFieldIndex = description->BitShiftedIndices[defineEntry];
				if ((shaderIndex & bitFieldIndex) == bitFieldIndex)
				{
					return true;
				}
				else
				{
					
					std::vector<std::string>* defindedWhen = &(description->DefinedWhen[defineEntry]);

					bool isDefined = false;

					for (size_t i = 0, l = defindedWhen->size(); i < l; ++i)
					{
						isDefined = ResolveDefinedWhen(defindedWhen->at(i), description, shaderIndex, alreadyProcessedProperties);
					}

					return isDefined;
				}
			}
			else
			{
				// in this case we are dealing with just an uniform (aka. material property)
				// Check if it is already present.
				if (std::find(alreadyProcessedProperties->begin(), alreadyProcessedProperties->end(), defineEntry) != alreadyProcessedProperties->end())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
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
			std::vector<std::string> shaderDescriptions = { "MeshDefaultShader.json", "MeshBasicShader.json" };

			for (size_t i = 0, l = shaderDescriptions.size(); i < l; ++i)
			{
				ShaderDescription* shaderDescription = ShaderParser::GetInstance()->Parse(baseDirectoryPath + shaderDescriptions[i]);
				uint64 shaderCounter = 0;

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

					// Create bitfield values for all the define-options
					// This stays constant for ALL of this shader's permutation.
					// Todo: We need to store this look up table somewhere
					for (size_t si = 0, sl = indexedDefinedWhen.size(); si < sl; ++si)
					{
						std::string defineOption = indexedDefinedWhen.at(si);
						uint64 bitShiftedIdx = BITSHIFT(static_cast<uint64>(si));
						shaderDescription->BitShiftedIndices[defineOption] = bitShiftedIdx;
					}

					// Save the description for later use, when we need to select a shader-instance based on the material inputs
					ShaderDescriptions[shaderDescription->Name] = shaderDescription;

					// Prepare boolean combinations for 'DefinedWhen'-Values
					uint32 definedWhenCount = static_cast<uint32>(indexedDefinedWhen.size());

					// TODO: u starts at 1, since we don't care about shaders, which have nothing defined!
					for (uint32 u = 1, ul = static_cast<uint32>(pow(2, definedWhenCount)); u < ul; ++u)
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
						typedef std::unordered_map<std::string, bool>::iterator DefinedWhenIterator;
						for (DefinedWhenIterator iterator = definedWhenOptions.begin(); iterator != definedWhenOptions.end(); ++iterator)
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
							// Generate an unique identifer for this permuation of the shader, based on it's defined values
							uint64 shaderIndex = 0;
							
							for (DefinedWhenIterator iterator = definedWhenOptions.begin(); iterator != definedWhenOptions.end(); ++iterator)
							{	
								std::string defineName = iterator->first;
								bool isDefined = iterator->second;

								if (isDefined)
								{
									uint64 bitShiftedIndex = shaderDescription->BitShiftedIndices[defineName];
									shaderIndex |= bitShiftedIndex;
								}
							}

							Shader* shader = new Shader();
							shader->SetName(shaderDescription->Name + " [" + std::to_string(shaderIndex) + "]");
							shader->LoadShaderFromFile(Shader::VERTEX_SHADER, vertexShaderPath, compileOptions);
							shader->LoadShaderFromFile(Shader::PIXEL_SHADER, pixelShaderPath, compileOptions);
							shader->CompileShader();

							// Store the shader inside the Shaders-Map
							(Shaders[shaderDescription->Name])[shaderIndex] = shader;

							shaderCounter++;
						}
					}
				}

				SHF_PRINTF("Created %llu permutations for %s \n", shaderCounter, shaderDescription->Name.c_str());

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

			{
				typedef std::unordered_map<uint64, Shader*>::iterator it_type;
				for (it_type iterator = MeshShaders.begin(); iterator != MeshShaders.end(); ++iterator) {
					Shader* shader = iterator->second;
					shader->Destroy();
					delete shader;
				}
			}

			{
				typedef std::unordered_map<std::string, ShaderDescription*>::iterator it_type;
				for (it_type iterator = ShaderDescriptions.begin(); iterator != ShaderDescriptions.end(); ++iterator)
				{
					delete iterator->second;
				}
				ShaderDescriptions.clear();
			}

			{
				typedef std::unordered_map<std::string, std::unordered_map<uint64, Shader*>>::iterator ShaderMapLevel0Iterator;
				typedef std::unordered_map<uint64, Shader*>::iterator ShaderMapLevel1Iterator;

				for (ShaderMapLevel0Iterator iteratorLevel0 = Shaders.begin(); iteratorLevel0 != Shaders.end(); ++iteratorLevel0)
				{
					auto shaderMapLevel1 = iteratorLevel0->second;

					for (ShaderMapLevel1Iterator iteratorLevel1 = shaderMapLevel1.begin(); iteratorLevel1 != shaderMapLevel1.end(); ++iteratorLevel1)
					{
						Shader* shader = iteratorLevel1->second;
						shader->Destroy();
						delete shader;
					} shaderMapLevel1.clear();
				}
			}
		}

		ShaderLibrary* ShaderLibrary::GetInstance()
		{
			static ShaderLibrary* instance(new ShaderLibrary);
			return instance;
		}

		std::unordered_map<std::string, ShaderDescription*>* ShaderLibrary::GetShaderDescriptions()
		{
			return &ShaderDescriptions;
		}

		ShaderDescription* ShaderLibrary::GetShaderDescription(Shader* shader)
		{
			if (shader != nullptr && ShaderDescriptions.find(shader->GetName()) != ShaderDescriptions.end())
			{
				return ShaderDescriptions[shader->GetName()];
			} 

			return nullptr;
		}

		std::unordered_map<uint64, Shader*>* ShaderLibrary::GetShaders(ShaderDescription* shaderDescription)
		{
			return &Shaders[shaderDescription->Name];
		}

		std::unordered_map<uint64, Shader*>* ShaderLibrary::GetShaders(std::string baseShaderName)
		{
			return &Shaders[baseShaderName];
		}
	};
};
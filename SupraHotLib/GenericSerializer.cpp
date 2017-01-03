#include "GenericSerializer.h"
#include "ShaderMaterial.h"
#include <json11/json11.hpp>
#include "BooleanMaterialProperty.h"
#include "TextureCubeMaterialProperty.h"
#include "Texture2DMaterialProperty.h"
#include "FloatMaterialProperty.h"
#include "Vec2MaterialProperty.h"
#include "Vec3MaterialProperty.h"
#include "Vec4MaterialProperty.h"
#include "FileSystem.h"
#include "FileReader.h"
#include "ShaderParser.h"

namespace SupraHot
{
	namespace Utils
	{
		GenericSerializer::GenericSerializer(std::string pathToFile)
		{
			this->PathToFile = pathToFile;
			Modes[0] = "rb";
			Modes[1] = "r";
			Modes[2] = "w+b";
			Modes[3] = "w+";
		
		}

		GenericSerializer::~GenericSerializer()
		{

		}

		void GenericSerializer::OpenFile(MODE mode)
		{

			if (mode < 2)
			{
				if (FileSystem::GetInstance()->FileExists("", PathToFile))
				{
					File = FileSystem::GetInstance()->GetFile("", PathToFile, Modes[mode]);

					if (File != nullptr)
					{
						FileOpened = true;
					}
				}
			}
			else
			{
				File = FileSystem::GetInstance()->GetFile("", PathToFile, Modes[mode]);

				if (File != nullptr)
				{
					FileOpened = true;
				}
			}
			
		}

		void GenericSerializer::CloseFile()
		{
			if (FileOpened)
			{
				std::fclose(File);
				FileOpened = false;
			}
		}

		void GenericSerializer::Serialize()
		{
		}

		void GenericSerializer::Deserialize()
		{
		}

		void GenericSerializer::Serialize(Graphics::ShaderMaterial* shaderMaterial)
		{
			std::string materialName = shaderMaterial->Name;
			std::string shaderDescriptionName = shaderMaterial->GetShaderDescription()->Name;
			std::string shaderDescriptionFileName = shaderMaterial->GetShaderDescription()->FileName;
			std::vector<Graphics::MaterialProperty*>* materialProperties = shaderMaterial->GetMaterialProperties();

			json11::Json::array materialPropertiesJsonArray;

			for (size_t i = 0, l = materialProperties->size(); i < l; ++i)
			{
				Graphics::MaterialProperty* materialProperty = materialProperties->at(i);

				materialPropertiesJsonArray.push_back(json11::Json::object {
						{
							"Name", materialProperty->GetName()
						},
						{
							"Type", materialProperty->GetType()
						},

						{
							"Value", GetValueForMaterialProperty(materialProperty)
						}
				});
			}

			json11::Json materialFileJson = json11::Json::object
			{
				{
					"MaterialName", materialName
				},

				{
					"ShaderDescription", shaderDescriptionName
				},

				{ 
					"ShaderDescriptionFileName", shaderDescriptionFileName
				},

				{ 
					"MaterialProperties", materialPropertiesJsonArray
				}
			};

		
			std::string JsonString = materialFileJson.dump();

			SHF_PRINTF("Serialize %s\n", shaderMaterial->Name.c_str());
			SHF_PRINTF("%s\n", JsonString.c_str());

			OpenFile(Utils::WRITE_PLAIN);
			
			if (FileOpened)
			{
				std::fputs(JsonString.c_str(), File);
			}

			CloseFile();
		}

		void GenericSerializer::Deserialize(Graphics::ShaderMaterial& shaderMaterial)
		{
			// TODO: load from disk
			OpenFile(READ_PLAIN);

			if (FileOpened)
			{
				CloseFile();

				std::vector<std::string> fileContent = SupraHot::Utils::FileReader::GetInstance()->ReadFile(PathToFile);

#if DEVELOPMENT == 1
				SHF_PRINTF("FileContent size = %llu \n", fileContent.size());
#endif
				std::string jsonFile = "";
				for (std::string s : fileContent)
				{
					jsonFile += s;
				}

				std::string err;
				auto json = json11::Json::parse(jsonFile.c_str(), err);


				std::string materialName = json["MaterialName"].string_value();
				std::string shaderDescriptionName = json["ShaderDescription"].string_value();
				std::string shaderDescriptionFileName = json["ShaderDescriptionFileName"].string_value();

				// Check if shader description exists already in ShaderLibrary.
				std::unordered_map<std::string, Graphics::ShaderDescription*>* shaderDescriptions = Graphics::ShaderLibrary::GetInstance()->GetShaderDescriptions();
				if (shaderDescriptions->find(shaderDescriptionName) != shaderDescriptions->end())
				{
					shaderMaterial.SetShaderDescription(shaderDescriptions->at(shaderDescriptionName));
#if DEVELOPMENT == 1
					SHF_PRINTF("Found shader description: %s in cache! \n", shaderDescriptionName.c_str());
#endif
				} 
				else // Try to load the shader descrip. from disk
				{
#if DEVELOPMENT == 1
					SHF_PRINTF("Not in cache: %s for %s \n", shaderDescriptionFileName.c_str(), materialName.c_str());
#endif

					if (FileSystem::GetInstance()->FileExists("Shaders/Description/", shaderDescriptionFileName))
					{
						Graphics::ShaderLibrary::GetInstance()->ProcessShaderDescription(Graphics::ShaderParser::GetInstance()->Parse("Shaders/Description/" + shaderDescriptionFileName));
						shaderMaterial.SetShaderDescription(shaderDescriptions->at(shaderDescriptionName));
					}
					else
					{
						// Crash!
						SHF_PRINTF("Could not load the shader description: %s for %s \n", shaderDescriptionFileName.c_str(), materialName.c_str());
#if PLATFORM_WINDOWS
						_exit(0);
#endif
					}

				}

				// Read the MaterialProperties array and create MaterialProperties-Objects and add them to the material!
				if (shaderMaterial.GetShaderDescription() != nullptr)
				{
					if (json["MaterialProperties"].is_array())
					{
						auto materialProperties = json["MaterialProperties"].array_items();
						for (size_t i = 0, l = materialProperties.size(); i < l; ++i)
						{
							if (materialProperties.at(i).is_object())
							{
								auto entry = materialProperties.at(i);
								std::string entryName = entry["Name"].string_value();
								std::string entryType = entry["Type"].string_value();
								auto entryValue = entry["Value"];

								if (entryType == "Boolean")
								{
									bool value = false;

									if (entryValue.is_bool())
									{
										value = entryValue.bool_value();
									}

									auto materialProperty = new Graphics::BooleanMaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else if (entryType == "Float")
								{
									float value = 0.0f;

									if (entryValue.is_number())
									{
										value = static_cast<float>(entryValue.number_value());
									}
									else
									{
										value = std::stof(entryValue.string_value());
									}

									// Generate new floatProperty and add it to the shadermaterial
									auto materialProperty = new Graphics::FloatMaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else if (entryType == "Vec2")
								{
									Vec2 value;

									if (entryValue["X"].is_number())
									{
										value.x = static_cast<float>(entryValue["X"].number_value());
									}
									else
									{
										value.x = std::stof(entryValue["X"].string_value());
									}

									if (entryValue["Y"].is_number())
									{
										value.y = static_cast<float>(entryValue["Y"].number_value());
									}
									else
									{
										value.y = std::stof(entryValue["Y"].string_value());
									}

									auto materialProperty = new Graphics::Vec2MaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else if (entryType == "Vec3")
								{
									Vec3 value;

									if (entryValue["X"].is_number())
									{
										value.x = static_cast<float>(entryValue["X"].number_value());
									}
									else
									{
										value.x = std::stof(entryValue["X"].string_value());
									}

									if (entryValue["Y"].is_number())
									{
										value.y = static_cast<float>(entryValue["Y"].number_value());
									}
									else
									{
										value.y = std::stof(entryValue["Y"].string_value());
									}


									if (entryValue["Z"].is_number())
									{
										value.z = static_cast<float>(entryValue["Z"].number_value());
									}
									else
									{
										value.z = std::stof(entryValue["Z"].string_value());
									}

									auto materialProperty = new Graphics::Vec3MaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else if (entryType == "Vec4")
								{
									Vec4 value;

									if (entryValue["X"].is_number())
									{
										value.x = static_cast<float>(entryValue["X"].number_value());
									}
									else
									{
										value.x = std::stof(entryValue["X"].string_value());
									}

									if (entryValue["Y"].is_number())
									{
										value.y = static_cast<float>(entryValue["Y"].number_value());
									}
									else
									{
										value.y = std::stof(entryValue["Y"].string_value());
									}

									if (entryValue["Z"].is_number())
									{
										value.z = static_cast<float>(entryValue["Z"].number_value());
									}
									else
									{
										value.z = std::stof(entryValue["Z"].string_value());
									}

									if (entryValue["W"].is_number())
									{
										value.w = static_cast<float>(entryValue["W"].number_value());
									}
									else
									{
										value.w = std::stof(entryValue["W"].string_value());
									}

									auto materialProperty = new Graphics::Vec4MaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else if (entryType == "Texture2D")
								{
									std::string value = "";

									if (entryValue.is_string())
									{
										value = entryValue.string_value();
									}

									auto materialProperty = new Graphics::Texture2DMaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else if (entryType == "TextureCube")
								{
									std::string value = "";

									if (entryValue.is_string())
									{
										value = entryValue.string_value();
									}

									auto materialProperty = new Graphics::TextureCubeMaterialProperty(entryName);
									materialProperty->SetValue(value);
									shaderMaterial.AddMaterialProperty(materialProperty);
								}
								else
								{
#if DEVELOPMENT == 1
									SHF_PRINTF("GenericSerializer::Deserialize(); UNKNOWN MaterialProperty Type \n");
#endif
								}
							}
						}
					}
				}


				shaderMaterial.Name = materialName;

			}
		}

		json11::Json GenericSerializer::GetValueForMaterialProperty(Graphics::MaterialProperty* materialProperty)
		{
			json11::Json::object obj {};

			if (materialProperty->GetType() == "Boolean")
			{
				return static_cast<Graphics::BooleanMaterialProperty*>(materialProperty)->GetValue();
			}
			else if (materialProperty->GetType() == "Float")
			{
				return std::to_string(static_cast<Graphics::FloatMaterialProperty*>(materialProperty)->GetValue());
			}
			else if (materialProperty->GetType() == "Vec2")
			{
				Vec2 value = static_cast<Graphics::Vec2MaterialProperty*>(materialProperty)->GetValue();
				obj.insert({"X", std::to_string(value.x) });
				obj.insert({"Y", std::to_string(value.y) });
				return obj;
			}
			else if (materialProperty->GetType() == "Vec3")
			{
				Vec3 value = static_cast<Graphics::Vec3MaterialProperty*>(materialProperty)->GetValue();
				obj.insert({ "X", std::to_string(value.x) });
				obj.insert({ "Y", std::to_string(value.y) });
				obj.insert({ "Z", std::to_string(value.z) });
				return obj;
			}
			else if (materialProperty->GetType() == "Vec4")
			{
				Vec4 value = static_cast<Graphics::Vec4MaterialProperty*>(materialProperty)->GetValue();
				obj.insert({ "X", std::to_string(value.x) });
				obj.insert({ "Y", std::to_string(value.y) });
				obj.insert({ "Z", std::to_string(value.z) });
				obj.insert({ "W", std::to_string(value.w) });
				return obj;
			}
			else if (materialProperty->GetType() == "Texture2D")
			{
				std::string path = static_cast<Graphics::Texture2DMaterialProperty*>(materialProperty)->GetValue();
				return path.c_str();
			}

			else if (materialProperty->GetType() == "TextureCube")
			{
				std::string path = static_cast<Graphics::TextureCubeMaterialProperty*>(materialProperty)->GetValue();
				return path.c_str();
			}

			return obj;
		}

	};
};
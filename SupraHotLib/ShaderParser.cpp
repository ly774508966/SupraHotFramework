#include "ShaderParser.h"
#include <json11/json11.hpp>
#include "FileReader.h"
#include <unordered_map>
#include "FileSystem.h"
#include "StringUtil.h"
#include "ShaderDescription.h"

namespace SupraHot
{
	namespace Graphics
	{
		
		ShaderParser::ShaderParser()
		{
		}

		ShaderParser* ShaderParser::GetInstance()
		{
			static ShaderParser* instance(new ShaderParser);
			return instance;
		}

		ShaderParser::~ShaderParser()
		{
		}

		// Members here

		void ShaderParser::ParseUniformsIntoMap(std::vector<std::string>& fileContents, std::unordered_map<std::string, std::string>& map)
		{
			for (size_t i = 0, l = fileContents.size(); i < l; ++i)
			{
				std::string line = fileContents.at(i);
				line = Utils::StringUtil::ltrim(line);

				// line begins with 'uniform'
				if (line.find("uniform") == 0)
				{
					std::vector<std::string> splitLine = Utils::StringUtil::Split(line, " ");
					std::string type = Utils::StringUtil::trim(splitLine[1]);
					std::string name = Utils::StringUtil::trim(splitLine[2]);

					// remove ';' from the name
					size_t idx = name.rfind(";");
					name = name.substr(0, idx);
					name = Utils::StringUtil::trim(name);

					if (map[name].length() <= 0)
					{
						map[name] = type;
					}
				}
			}
		}

		ShaderDescription* ShaderParser::Parse(std::string pathToShaderDefinitionFile)
		{
			SHF_PRINTF("pathToShaderDefinitionFile = %s \n", pathToShaderDefinitionFile.c_str());

			std::vector<std::string> fileContent = SupraHot::Utils::FileReader::GetInstance()->ReadFile(pathToShaderDefinitionFile);

			SHF_PRINTF("fileContent size = %llu \n", fileContent.size());

			std::string jsonFile = "";
			for (std::string s : fileContent)
			{
				jsonFile += s;
			}

			std::string err;
			auto json = json11::Json::parse(jsonFile.c_str(), err);

#if DEVELOPMENT == 1
			SHF_PRINTF("ShaderParser::Parse Error: %s \n", err.c_str());
#endif

			std::string shaderName = json["Description"]["Name"].string_value();
			std::string shaderDescription = json["Description"]["Description"].string_value();

			std::string brdfType = json["BRDF"].string_value();
			Shader::BRDFType brdf = Shader::BRDFType::None;

			if (brdfType.length() > 0)
			{
				if (brdfType.find("GGX") != -1)
				{
					brdf = Shader::BRDFType::GGX;
				}
				else if (brdfType.find("Skin") != -1)
				{
					brdf = Shader::BRDFType::Skin;
				}
				else if (brdfType.find("Cloth") != -1)
				{
					brdf = Shader::BRDFType::Cloth;
				}
				else if (brdfType.find("Hair") != -1)
				{
					brdf = Shader::BRDFType::Hair;
				}
				else if (brdfType.find("Eye") != -1)
				{
					brdf = Shader::BRDFType::Eye;
				}
			}


			std::string vertexShaderPath = json["Files"]["GL"]["VertexShader"].string_value();
			std::string pixelShaderPath = json["Files"]["GL"]["PixelShader"].string_value();

#ifdef PLATFORM_ANDROID
			vertexShaderPath = json["Files"]["GLES3"]["VertexShader"].string_value();
			pixelShaderPath = json["Files"]["GLES3"]["PixelShader"].string_value();
#endif

			bool vertexShaderExists = false;
			bool pixelShaderExists = false;

			if (Utils::FileSystem::GetInstance()->FileExists("", vertexShaderPath))
			{
				vertexShaderExists = true;
			}
			else if (Utils::FileSystem::GetInstance()->FileExists(
					"", 
					Utils::StringUtil::GetPathFromFilePath(pathToShaderDefinitionFile) + vertexShaderPath
				))
			{
				vertexShaderExists = true;
				vertexShaderPath = Utils::StringUtil::GetPathFromFilePath(pathToShaderDefinitionFile) + vertexShaderPath;
			} else
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("Vertex shader not found \n");
#endif
				return nullptr;
			}

			if (Utils::FileSystem::GetInstance()->FileExists("", pixelShaderPath))
			{
				pixelShaderExists = true;
			}
			else if (Utils::FileSystem::GetInstance()->FileExists("",
				Utils::StringUtil::GetPathFromFilePath(pathToShaderDefinitionFile) + pixelShaderPath))
			{
				pixelShaderExists = true;
				pixelShaderPath = Utils::StringUtil::GetPathFromFilePath(pathToShaderDefinitionFile) + pixelShaderPath;
			} else
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("Pixel shader not found \n");
#endif
				return nullptr;
			}
			
#if DEVELOPMENT == 1
			if (vertexShaderExists && pixelShaderExists)
			{
				SHF_PRINTF("Shader files exist \n");
			}
#endif 

			// Get defines from the json file
			std::string defines = json["Defines"].string_value();
			bool buildAllUniformDefines = json["Defines"]["BuildAllUniformDefines"].bool_value();

			// build up the defined when map. if we need to
			std::unordered_map<std::string, std::vector<std::string>> definedWhenMap;
			
			if (json["Defines"]["DefinedWhen"].is_object())
			{
				auto definedWhen = json["Defines"]["DefinedWhen"].object_items();
				typedef std::map<std::string, json11::Json>::iterator it_type;
				for (it_type iterator = definedWhen.begin(); iterator != definedWhen.end(); ++iterator)
				{

					std::string key = iterator->first;
#if DEVELOPMENT == 1
					SHF_PRINTF("%s is defined when .... \n", key.c_str());
#endif
					if (iterator->second.is_array())
					{
						auto definedArray = iterator->second.array_items();

						for (size_t i = 0, l = definedArray.size(); i < l; ++i)
						{
							if (definedArray.at(i).is_string())
							{
								std::string value = definedArray.at(i).string_value();
#if DEVELOPMENT == 1
								SHF_PRINTF("-> %s is defined \n", value.c_str());
#endif
								definedWhenMap[key].push_back(value);
							}
						}
					}
				}
			}

			// Parse all uniforms for later use. Needed when we create material properties
			std::vector<std::string> vertexShaderContents = Utils::FileReader::GetInstance()->ReadFile(vertexShaderPath);
			std::vector<std::string> pixelShaderContents = Utils::FileReader::GetInstance()->ReadFile(pixelShaderPath);
			std::unordered_map<std::string, std::string> uniformNameToUniformTypeMap;

			ParseUniformsIntoMap(vertexShaderContents, uniformNameToUniformTypeMap);
			ParseUniformsIntoMap(pixelShaderContents, uniformNameToUniformTypeMap);

#if DEVELOPMENT == 1
			// Sanity test
			typedef std::unordered_map<std::string, std::string>::iterator it_type;
			for (it_type iterator = uniformNameToUniformTypeMap.begin(); iterator != uniformNameToUniformTypeMap.end(); ++iterator)
			{
				SHF_PRINTF("Uniform: %s -> %s \n", iterator->first.c_str(), iterator->second.c_str());
			}
#endif

			// Fill the definedWhenMap with all those uniforms,  if buildAllUniforms is set to true 
			if (buildAllUniformDefines)
			{
				typedef std::unordered_map<std::string, std::string>::iterator it_type;
				for (it_type iterator = uniformNameToUniformTypeMap.begin(); iterator != uniformNameToUniformTypeMap.end(); ++iterator)
				{
					if (definedWhenMap["_" + iterator->first].size() <= 0)
					{
						definedWhenMap["_" + iterator->first].push_back(iterator->second);
					}
				}
			}
			
			// Build dependencies map
			std::unordered_map<std::string, std::vector<std::string>> dependenciesMap;
			if (json["Dependencies"].is_object())
			{
				auto dependencies = json["Dependencies"].object_items();
				typedef std::map<std::string, json11::Json>::iterator it_type;
				for (it_type iterator = dependencies.begin(); iterator != dependencies.end(); ++iterator)
				{
					std::string key = iterator->first;
#if DEVELOPMENT == 1
					SHF_PRINTF("%s depends on ... \n", key.c_str());
#endif
					if (iterator->second.is_array())
					{
						auto dependenciesArray = iterator->second.array_items();

						for (size_t i = 0, l = dependenciesArray.size(); i < l; ++i)
						{
							if (dependenciesArray.at(i).is_string())
							{
								std::string dependency = dependenciesArray.at(i).string_value();
#if DEVELOPMENT == 1
								SHF_PRINTF("-> %s  \n", dependency.c_str());
#endif
								dependenciesMap[key].push_back(dependency);
							}
						}
					}
				}
			}

			// Create injected uniforms
			std::unordered_map<std::string, std::string> injectedUniformsNameToTypeMap;
			if (json["Inject"].is_array())
			{
				auto injectedUniforms = json["Inject"].array_items();

				for (size_t i = 0, l = injectedUniforms.size(); i < l; ++i)
				{
					std::string injectName = injectedUniforms.at(i).string_value();
					std::string injectType = "UNKOWN";

					if (uniformNameToUniformTypeMap.find(injectName) != uniformNameToUniformTypeMap.end())
					{
						injectType = uniformNameToUniformTypeMap[injectName];

						injectedUniformsNameToTypeMap[injectName] = injectType;
					}

#if DEVELOPMENT == 1
					SHF_PRINTF("Inject -> %s = %s \n", injectName.c_str(), injectType.c_str());
#endif
				}
			}

			// This is experimental

			// description->Inject
			if (injectedUniformsNameToTypeMap.size() > 0){
				typedef std::unordered_map<std::string, std::string>::iterator iterator;
				for (iterator it = injectedUniformsNameToTypeMap.begin(); it != injectedUniformsNameToTypeMap.end(); ++it)
				{
					SHF_PRINTF("Removing %s \n", it->first.c_str());
					if (uniformNameToUniformTypeMap.find(it->first) != uniformNameToUniformTypeMap.end())
					{
						uniformNameToUniformTypeMap.erase(it->first);
						SHF_PRINTF("... OK. \n");
					}
				}
			}

			ShaderDescription* description = new ShaderDescription();
			description->Name = shaderName;
			description->FileName = Utils::StringUtil::GetFileNameFromPath(pathToShaderDefinitionFile);
			description->Description = shaderDescription;
			description->BRDFType = brdf;
			description->Dependencies = dependenciesMap;
			description->DefinedWhen = definedWhenMap;
			description->Uniforms = uniformNameToUniformTypeMap;
			description->Inject = injectedUniformsNameToTypeMap;
			description->VertexShaderPath = vertexShaderPath;
			description->PixelShaderPath = pixelShaderPath;

			return description;
		}
	};
};

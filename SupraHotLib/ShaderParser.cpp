#include "ShaderParser.h"
#include <json11/json11.hpp>
#include "FileReader.h"
#include <unordered_map>
#include "FileSystem.h"
#include <filesystem>
#include "StringUtil.h"

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

		void ShaderParser::Parse(std::string pathToShaderDefinitionFile)
		{
			std::vector<std::string> fileContent = SupraHot::Utils::FileReader::GetInstance()->ReadFile(pathToShaderDefinitionFile);
			std::string jsonFile = "";
			for (std::string s : fileContent)
			{
				jsonFile += s;
			}

			std::string err;
			auto json = json11::Json::parse(jsonFile.c_str(), err);

#if DEVELOPMENT == 1
			SHF_PRINTF("Error: %s \n", err.c_str());
#endif

			std::string shaderName = json["Description"]["Name"].string_value();
			std::string shaderDescription = json["Description"]["Description"].string_value();

			std::string vertexShaderPath = json["Files"]["VertexShader"].string_value();
			std::string pixelShaderPath = json["Files"]["PixelShader"].string_value();

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
				return;
			}

			if (Utils::FileSystem::GetInstance()->FileExists("", pixelShaderPath))
			{
				pixelShaderExists = true;
			}
			else if (Utils::FileSystem::GetInstance()->FileExists(
				"",
				Utils::StringUtil::GetPathFromFilePath(pathToShaderDefinitionFile) + pixelShaderPath
				))
			{
				pixelShaderExists = true;
				pixelShaderPath = Utils::StringUtil::GetPathFromFilePath(pathToShaderDefinitionFile) + pixelShaderPath;
			} else
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("Pixel shader not found \n");
#endif
				return;
			}
			
#if DEVELOPMENT == 1
			if (vertexShaderExists && pixelShaderExists)
			{
				SHF_PRINTF("Shader files exist \n");
			}
#endif 

			// defines
			std::string defines = json["Defines"].string_value();
			bool buildAllUniformDefines = json["Defines"]["BuildAllUniformDefines"].bool_value();

			// build up the defined when map. if we need to
			std::unordered_map<std::string, std::vector<std::string>> definedWhenMap;
			
			if (!buildAllUniformDefines)
			{
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
			} // end (!buildAllUniformDefines)
			else
			{

				// Build defines for all uniforms here....
				// Just skip internals, like ModelViewMatrix and stuff.


				// Note: Shaderlibrary should call into this method and get a struct back with
				// all the necessary informations

	/*			struct ShaderDescription
				{
					std::string Name = "";
					std::string Description = "";

					std::string VertexShaderPath = "";
					std::string PixelShaderPath = "";

					// deps
					// defines
				};

				ShaderDescription shaderDesc = {};
				shaderDesc.Name = "wurst";*/

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


		}

		void ShaderParser::ParseGLShader(std::string pathToShaderFile)
		{
		}

		void ShaderParser::ParseJSON(std::string pathToJson)
		{
		}
	};
};

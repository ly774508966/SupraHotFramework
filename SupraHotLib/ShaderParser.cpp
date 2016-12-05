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


				std::vector<std::string> vertexShaderContents = Utils::FileReader::GetInstance()->ReadFile(vertexShaderPath);
				std::vector<std::string> pixelShaderContents = Utils::FileReader::GetInstance()->ReadFile(pixelShaderPath);
				std::unordered_map<std::string, std::string> uniformNameToUniformTypeMap;

				// Process shader source code.
				/*
				
					// Find Input&Output from VS, FS
					if (characterAfterKeyword == ' ')
					{
						if (keyword == "layout")
						{
							printf("LAYOUT KEYWORD: %s \n", currentLine.c_str());
						} 
						else
						{
							ShaderUIElement* uiElement = new ShaderUIElement();
							std::vector<std::string> lineParts = StringUtil::Split(currentLine, " ", false);	
							uiElement->Keyword = lineParts[0];
							uiElement->Type = lineParts[1];
							uiElement->Name = lineParts[2].substr(0, lineParts[2].size() - 1);
							uiElement->Location = "-1";
							uiElement->LineNumber = currentLineNumber;
							result.push_back(uiElement);
						}
					} 
				
				*/

				for (size_t i = 0, l = vertexShaderContents.size(); i < l; ++i)
				{
					
					std::string line = vertexShaderContents.at(i);
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

						SHF_PRINTF("Uniform -> [%s]==[%s] \n", type.c_str(), name.c_str());

						// Also in theory we would dynamically inject the needed unifroms like matrices.
						// we could store them on the shader, just like the material-properties.
						// they would get pushed onto the shader every frame and for every mesh we render
						// except for viewmatrix and projection matrix, since it will stay there for eternity,
						// until we override it.


						if (uniformNameToUniformTypeMap[name].length() <= 0)
						{
							SHF_PRINTF("Inject our uniform into the map \n");
							uniformNameToUniformTypeMap[name] = type;
						}

					}

				}


				// create a method for this stuff....

				for (size_t i = 0, l = pixelShaderContents.size(); i < l; ++i)
				{

					std::string line = pixelShaderContents.at(i);
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

						SHF_PRINTF("Uniform -> [%s]==[%s] \n", type.c_str(), name.c_str());

						// Also in theory we would dynamically inject the needed unifroms like matrices.
						// we could store them on the shader, just like the material-properties.
						// they would get pushed onto the shader every frame and for every mesh we render
						// except for viewmatrix and projection matrix, since it will stay there for eternity,
						// until we override it.


						if (uniformNameToUniformTypeMap[name].length() <= 0)
						{
							SHF_PRINTF("Inject our uniform into the map \n");
							uniformNameToUniformTypeMap[name] = type;
						}

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


		}

		void ShaderParser::ParseGLShader(std::string pathToShaderFile)
		{
		}

		void ShaderParser::ParseJSON(std::string pathToJson)
		{
		}
	};
};

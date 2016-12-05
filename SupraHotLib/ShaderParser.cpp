#include "ShaderParser.h"
#include <json11/json11.hpp>
#include "FileReader.h"

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

			SHF_PRINTF("%s \n", jsonFile.c_str());

			std::string err;
			auto json = json11::Json::parse(jsonFile.c_str(), err);

			SHF_PRINTF("Error: %s \n", err.c_str());

			std::string shaderName = json["Description"]["Name"].string_value();
			std::string shaderDescription = json["Description"]["Description"].string_value();

			std::string vertexShaderPath = json["Files"]["VertexShader"].string_value();
			std::string pixelShaderPath = json["Files"]["PixelShader"].string_value();

			// defines
			std::string defines = json["Defines"].string_value();
			bool buildAllUniformDefines = json["Defines"]["BuildAllUniformDefines"].bool_value();
			std::string definedWhen = json["Defines"]["DefinedWhen"].string_value();

			printf("%s \n", shaderName.c_str());
			printf("%s \n", shaderDescription.c_str());
			printf("%s \n", vertexShaderPath.c_str());
			printf("%s \n", pixelShaderPath.c_str());
			printf("defines = %s \n", defines.c_str());
			printf("defines = %d \n", json["Defines"].is_object());
			printf("build all = %d \n", buildAllUniformDefines);
			printf("definedWhen = %s \n", definedWhen.c_str());

		}

		void ShaderParser::ParseGLShader(std::string pathToShaderFile)
		{
		}

		void ShaderParser::ParseJSON(std::string pathToJson)
		{
		}
	};
};

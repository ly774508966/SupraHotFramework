#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class ShaderParser
		{
		private:
			ShaderParser();
		public:
			static ShaderParser* GetInstance();
			~ShaderParser();


			// Parses the global file
			void Parse(std::string pathToShaderDefinitionFile);
			void ParseGLShader(std::string pathToShaderFile);
			void ParseJSON(std::string pathToJson);
		};
	};
};
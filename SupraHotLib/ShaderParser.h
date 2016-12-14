#pragma once
#include "Platform.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class ShaderDescription;

		class ShaderParser
		{
		private:
			ShaderParser();

			void ParseUniformsIntoMap(std::vector<std::string>& fileContents, std::unordered_map<std::string, std::string>& map);

		public:
			static ShaderParser* GetInstance();
			~ShaderParser();


			// Parses the global file
			// TODO: Delete the shader description.
			ShaderDescription* Parse(std::string pathToShaderDefinitionFile);
		};
	};
};
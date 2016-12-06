#pragma once
#include "Platform.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class ShaderParser
		{
		public:
			class ShaderDescription 
			{
			public:
				std::string Name = "";
				std::string Description = "";
				std::string VertexShaderPath = "";
				std::string PixelShaderPath = "";
				std::unordered_map<std::string, std::vector<std::string>> DefinedWhen = {};
				std::unordered_map<std::string, std::vector<std::string>> Dependencies = {};
				std::unordered_map<std::string, std::string> Uniforms = {};
				
				ShaderDescription(){};
				~ShaderDescription(){};
			};


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
#pragma once
#include "Platform.h"
#include "Shader.h"
#include <unordered_map>

namespace SupraHot
{

	namespace Graphics
	{
		class MaterialInputs;
		class MeshData;
		class ShaderDescription;

		class ShaderLibrary
		{
			
		public:
			enum class SkyboxShader
			{
				CubeMap = 0,
				SphereMap,

				Count
			};

			enum class ScreenSpace
			{
				RenderToScreen = 0,
				
				Count
			};

			static ShaderLibrary* GetInstance();
			std::unordered_map<std::string, ShaderDescription*>* GetShaderDescriptions();
			ShaderDescription* GetShaderDescription(Shader* shader);
			std::unordered_map<uint64, Shader*>* GetShaders(ShaderDescription* shaderDescription);
			std::unordered_map<uint64, Shader*>* GetShaders(std::string baseShaderName);

			~ShaderLibrary();

			Shader* Skybox[uint32(SkyboxShader::Count)] = {};
			Shader* ScreenSpace[uint32(ScreenSpace::Count)] = {};
			
			Shader* SelectShaderForShaderMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::MaterialInputs* material);

			void Initialize();
			void Destroy();

			bool ProcessShaderDescription(ShaderDescription* shaderDescription);

		private:
			std::unordered_map<std::string, ShaderDescription*> ShaderDescriptions;

			// Maps a shader name (could be an uuid in the future) to another map of uint to shader
			std::unordered_map<std::string, std::unordered_map<uint64, Shader*>> Shaders;
			ShaderLibrary();

			bool ResolveDefinedWhen(std::string defineEntry, ShaderDescription* description, uint64 shaderIndex, std::vector<std::string>* alreadyProcessedProperties);
			bool ResolveDependencies(std::string defineName, ShaderDescription* description, uint64 shaderIndex, std::vector<std::string>* alreadyProcessedProperties);
		};
	};
};
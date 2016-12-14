#pragma once
#include "Platform.h"
#include "Shader.h"
#include "Utility.h"
#include <unordered_map>

namespace SupraHot
{

	namespace Graphics
	{
		class ShaderMaterial;
		class Material;
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

			// For the G-Buffer
			enum class MeshVertexShader
			{
				// Position is always required. duh.
				Normal			 = BITSHIFT(0),
				UV				 = BITSHIFT(1),
				TangentBiTangent = BITSHIFT(2)
			};

			// For the G-Buffer
			enum class MeshPixelShader
			{
				AlbedoMap		= BITSHIFT(3),
				NormalMap		= BITSHIFT(4),
				RoughnessMap	= BITSHIFT(5),
				MetalnessMap	= BITSHIFT(6),
				SpecularMap		= BITSHIFT(7),
				ComboMap		= BITSHIFT(8)
			};


			static ShaderLibrary* GetInstance();
			const std::unordered_map<std::string, ShaderDescription*>& GetShaderDescriptions();
			ShaderDescription* GetShaderDescription(Shader* shader);
			std::unordered_map<uint64, Shader*>* GetShaders(ShaderDescription* shaderDescription);


			~ShaderLibrary();

			Shader* Skybox[uint32(SkyboxShader::Count)] = {};
			Shader* ScreenSpace[uint32(ScreenSpace::Count)] = {};
			std::unordered_map<uint64, Shader*> MeshShaders;

			Shader* SelectShaderForMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::Material* material);

			Shader* SelectShaderForShaderMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::ShaderMaterial* material);

			void Initialize();
			void Destroy();

		private:
			std::unordered_map<std::string, ShaderDescription*> ShaderDescriptions;

			// Maps a shader name (could be an uuid in the future) to another map of uint to shader
			std::unordered_map<std::string, std::unordered_map<uint64, Shader*>> Shaders;
			ShaderLibrary();
		};
	};
};
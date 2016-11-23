#pragma once
#include "Platform.h"
#include "Shader.h"
#include "Utility.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class Material;
		class MeshData;

		class ShaderLibrary
		{
			
		public:

			struct StaticMesh
			{
				// We only need the vertex shaders,
				// since we are going to render the mesh data to a g-buffer
				enum class VertexShader
				{
					Position = 0,
					PositionNormal,
					PositionUV,
					PositionNormalUV,
					PositionNormalUVTangentBiTangent,

					Count
				};
			};

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
			~ShaderLibrary();

			Shader* Skybox[uint32(SkyboxShader::Count)] = {};
			Shader* ScreenSpace[uint32(ScreenSpace::Count)] = {};
			Shader* MeshStatic[uint32(StaticMesh::VertexShader::Count)] = {};
			std::unordered_map<uint64, Shader*> MeshShaders;

			Shader* SelectShaderForMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::Material* material);

			void Initialize();
			void Destroy();

		private:
			ShaderLibrary();
		};
	};
};
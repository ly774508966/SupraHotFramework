#pragma once
#include "Platform.h"
#include "Shader.h"
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

			static ShaderLibrary* GetInstance();
			~ShaderLibrary();

			Shader* Skybox[uint32(SkyboxShader::Count)] = {};
			Shader* ScreenSpace[uint32(ScreenSpace::Count)] = {};
			Shader* MeshStatic[uint32(StaticMesh::VertexShader::Count)] = {};

			Shader* SelectShaderForMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::Material* material);

			void Initialize();
			void Destroy();

		private:
			ShaderLibrary();
		};
	};
};
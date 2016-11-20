#pragma once
#include "Platform.h"
#include "Shader.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class ShaderLibrary
		{
			
		public:

			struct VertexShader
			{
				enum StaticMesh
				{
					Position = 0,
					PositionNormal,
					PositionUV,
					PositionNormalUV,
					PositionNormalUVTangentBiTangent,

					Count
				};

			};

			struct PixelShader
			{
				enum Mesh
				{
					Lambert = 0,
					Phong,
					GGX,
					Cloth,
					Skin,

					Count
				};
			};

			enum SkyboxShader
			{
				CubeMap = 0,
				SphereMap,

				Count
			};

			static ShaderLibrary* GetInstance();
			~ShaderLibrary();

			Shader* Skybox[SkyboxShader::Count] = {};
			void Initialize();
			void Destroy();

		private:
			ShaderLibrary();
			Shader* MeshStatic[VertexShader::StaticMesh::Count] = {};
		};
	};
};
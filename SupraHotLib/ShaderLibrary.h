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

				enum Wurst
				{
					Start = StaticMesh::Count
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

		private:
			ShaderLibrary();
			Shader* MeshStaticVS[VertexShader::StaticMesh::Count];
			Shader* MeshPS[PixelShader::Mesh::Count];

		public:
			static ShaderLibrary* GetInstance();
			~ShaderLibrary();

			void Initialize();
			void Destroy();

			Shader* GetVertexShader(VertexShader identifer);
		};
	};
};
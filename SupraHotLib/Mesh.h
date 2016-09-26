#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class MeshData;
		class Material;

		class Mesh
		{
		private:
			Material* MeshMaterial;
			uint32 VertexHandle, UVHandle, NormalHandle, TangentHandle, BiNormalHandle;

			void Load(MeshData* meshData);
		public:
			Mesh();
			Mesh(MeshData* meshData);
			~Mesh();

			const uint32& GetVertexHandle();
			const uint32& GetUVHandle();
			const uint32& GetNormalHandle();
			const uint32& GetTangentHandle();
			const uint32& GetBiNormalHandle();

			Material* GetMaterial();
			void SetMaterial(Material* material);

			void Destroy();
		};
	};
};
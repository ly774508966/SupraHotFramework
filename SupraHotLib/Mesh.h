#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class MeshData;

		class Mesh
		{
		private:
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

			void Destroy();
		};
	};
};
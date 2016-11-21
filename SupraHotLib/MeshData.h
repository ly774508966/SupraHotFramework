#pragma once
#include "Platform.h"
#include <vector>
#include "Vec3.h"
#include "Vec2.h"
#include "Material.h"
#include "Face.h"

namespace SupraHot
{
	namespace Graphics
	{
		using namespace Math;

		class MeshData
		{
		public:
			uint32 VAOHandle;
			uint32 FaceCount;
			uint32 VertexCount;
			uint32 IndexBufferHandle;
			uint32 VertexBufferHandle;
			
			Vec3 Origin;
			std::string Name;
			Material Material;
			std::vector<Face> Faces; // Indices

			bool HasIndexData = false;
			bool HasPositionData = false;
			bool HasNormalData = false;
			bool HasBiTangentData = false;
			bool HasTangentData = false;
			bool HasUVData = false;

			uint32 GlIndexType;

			MeshData();
			~MeshData();
			void Destroy();
		};
	};
};


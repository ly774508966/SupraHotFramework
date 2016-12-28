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
			uint32 VAOHandle = 0;
			uint32 FaceCount = 0;
			uint32 VertexCount = 0;
			uint32 IndexBufferHandle = 0;
			uint32 VertexBufferHandle = 0;
			
			Vec3 Origin;
			std::string Name;
			std::vector<Face> Faces; // Indices

			bool HasIndexData = false;
			bool HasPositionData = false;
			bool HasNormalData = false;
			bool HasBiTangentData = false;
			bool HasTangentData = false;
			bool HasUVData = false;

			uint32 GlIndexType = GL_UNSIGNED_SHORT;

			MeshData();
			~MeshData();
			void Destroy();
		};
	};
};


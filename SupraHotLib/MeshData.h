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
			uint32 VAOId;
			uint32 FaceCount;
			uint32 IndexBufferHandle;
			uint32 PositionBufferHandle;
			uint32 NormalBufferHandle;
			uint32 BiNormalBufferHandle;
			uint32 TangentBufferHandle;
			uint32 UVBufferHandle;

			std::vector<uint32> IndexData;
			std::vector<Vec3> PositionData;
			std::vector<Vec3> NormalData;
			std::vector<Vec3> BiNormalData;
			std::vector<Vec3> TangentData;
			std::vector<Vec2> UVData;

			Vec3 Origin;
			std::string Name;
			Material MeshMaterial;
			std::vector<Face> Faces; // Indices

			bool HasIndexData = false;
			bool HasPositionData = false;
			bool HasNormalData = false;
			bool HasBiNormalData = false;
			bool HasTangentData = false;
			bool HasUVData = false;

			MeshData();
			~MeshData();
			void Destroy();
		};
	};
};


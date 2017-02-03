#pragma once
#include "Platform.h"
#include "Vec3.h"
#include "MeshData.h"

namespace SupraHot
{
	namespace Graphics
	{
		class AABB
		{
		private:
			Vec3 Minimum;
			Vec3 Maximum;
		public:
			AABB(MeshDataPtr& meshDataPtr);
			~AABB();

			const Vec3& GetMinimum();
			const Vec3& GetMaximum();
		};
	};
};
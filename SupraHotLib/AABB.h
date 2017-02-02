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
			Math::Vec3 Minimum;
			Math::Vec3 Maximum;
		public:
			AABB(MeshDataPtr& meshDataPtr);
			~AABB();
		};
	};
};
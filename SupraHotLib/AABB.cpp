#include "AABB.h"

namespace SupraHot
{
	namespace Graphics
	{
		AABB::AABB(MeshDataPtr& meshDataPtr)
		{
		}

		AABB::~AABB()
		{
		}

		const Vec3& AABB::GetMinimum()
		{
			return Minimum;
		}

		const Vec3& AABB::GetMaximum()
		{
			return Maximum;
		}
	};
};
#pragma once
#include "Platform.h"
#include "Vec3.h"

namespace SupraHot
{
	namespace Math{
		class Mat4;
	}

	namespace Graphics
	{
		using namespace Math;

		class AABB
		{
		private:
			Vec3 Minimum;
			Vec3 Maximum;
		public:
			AABB();
			AABB(Vec3 minimum, Vec3 maximum);
			~AABB();

			const Vec3& GetMinimum();
			const Vec3& GetMaximum();

			AABB Transform(Mat4 transformationMatrix);
		};
	};
};
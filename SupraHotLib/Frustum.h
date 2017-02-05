#pragma once
#include "Platform.h"
#include "Vec3.h"

namespace SupraHot
{
	namespace Math{
		class Quat4;
		class Mat4;
	}

	namespace Graphics
	{
		class AABB;

		class FrustumPlane
		{
		public:
			Math::Vec3 Normal;
			float DistanceToOrigin;

			FrustumPlane();
			~FrustumPlane();
			void Normalize();

			enum FrustumPlaneIndex
			{
				FRUSTUM_NEAR = 0,
				FRUSTUM_FAR = 1,
				FRUSTUM_LEFT = 2,
				FRUSTUM_RIGHT = 3,
				FRUSTUM_UP = 4,
				FRUSTUM_DOWN = 5
			};
		};

		class Frustum
		{
		public:
			Frustum();
			~Frustum();
			void Compute(Math::Mat4& viewProjectionMatrix);
			bool IntersectsSphere(Math::Vec3 sphereCenter, float sphereRadius);
			bool IntersectsAABB(AABB aabb, Math::Vec3 Position, Math::Vec3 scale, Math::Quat4 rotation);

			FrustumPlane Planes[6];
		};
	};
};
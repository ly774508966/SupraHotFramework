#include "Frustum.h"
#include "Mat4.h"
#include "AABB.h"

namespace SupraHot
{
	namespace Graphics
	{
		Frustum::Frustum()
		{
		}

		Frustum::~Frustum()
		{
		}

		void Frustum::Compute(Mat4& viewProjectionMatrix)
		{
			Planes[FrustumPlane::FRUSTUM_RIGHT].Normal.x		 = viewProjectionMatrix.m[0][3] - viewProjectionMatrix.m[0][0];
			Planes[FrustumPlane::FRUSTUM_RIGHT].Normal.y		 = viewProjectionMatrix.m[1][3] - viewProjectionMatrix.m[1][0];
			Planes[FrustumPlane::FRUSTUM_RIGHT].Normal.z		 = viewProjectionMatrix.m[2][3] - viewProjectionMatrix.m[2][0];
			Planes[FrustumPlane::FRUSTUM_RIGHT].DistanceToOrigin = viewProjectionMatrix.m[3][3] - viewProjectionMatrix.m[3][0];

			Planes[FrustumPlane::FRUSTUM_LEFT].Normal.x			 = viewProjectionMatrix.m[0][3] + viewProjectionMatrix.m[0][0];
			Planes[FrustumPlane::FRUSTUM_LEFT].Normal.y			 = viewProjectionMatrix.m[1][3] + viewProjectionMatrix.m[1][0];
			Planes[FrustumPlane::FRUSTUM_LEFT].Normal.z			 = viewProjectionMatrix.m[2][3] + viewProjectionMatrix.m[2][0];
			Planes[FrustumPlane::FRUSTUM_LEFT].DistanceToOrigin  = viewProjectionMatrix.m[3][3] + viewProjectionMatrix.m[3][0];

			Planes[FrustumPlane::FRUSTUM_DOWN].Normal.x			 = viewProjectionMatrix.m[0][3] + viewProjectionMatrix.m[0][1];
			Planes[FrustumPlane::FRUSTUM_DOWN].Normal.y			 = viewProjectionMatrix.m[1][3] + viewProjectionMatrix.m[1][1];
			Planes[FrustumPlane::FRUSTUM_DOWN].Normal.z			 = viewProjectionMatrix.m[2][3] + viewProjectionMatrix.m[2][1];
			Planes[FrustumPlane::FRUSTUM_DOWN].DistanceToOrigin  = viewProjectionMatrix.m[3][3] + viewProjectionMatrix.m[3][1];
			
			Planes[FrustumPlane::FRUSTUM_UP].Normal.x			 = viewProjectionMatrix.m[0][3] - viewProjectionMatrix.m[0][1];
			Planes[FrustumPlane::FRUSTUM_UP].Normal.y			 = viewProjectionMatrix.m[1][3] - viewProjectionMatrix.m[1][1];
			Planes[FrustumPlane::FRUSTUM_UP].Normal.z			 = viewProjectionMatrix.m[2][3] - viewProjectionMatrix.m[2][1];
			Planes[FrustumPlane::FRUSTUM_UP].DistanceToOrigin	 = viewProjectionMatrix.m[3][3] - viewProjectionMatrix.m[3][1];

			Planes[FrustumPlane::FRUSTUM_FAR].Normal.x			 = viewProjectionMatrix.m[0][3] - viewProjectionMatrix.m[0][2];
			Planes[FrustumPlane::FRUSTUM_FAR].Normal.y			 = viewProjectionMatrix.m[1][3] - viewProjectionMatrix.m[1][2];
			Planes[FrustumPlane::FRUSTUM_FAR].Normal.z			 = viewProjectionMatrix.m[2][3] - viewProjectionMatrix.m[2][2];
			Planes[FrustumPlane::FRUSTUM_FAR].DistanceToOrigin	 = viewProjectionMatrix.m[3][3] - viewProjectionMatrix.m[3][2];

			Planes[FrustumPlane::FRUSTUM_NEAR].Normal.x			 = viewProjectionMatrix.m[0][3] + viewProjectionMatrix.m[0][2];
			Planes[FrustumPlane::FRUSTUM_NEAR].Normal.y			 = viewProjectionMatrix.m[1][3] + viewProjectionMatrix.m[1][2];
			Planes[FrustumPlane::FRUSTUM_NEAR].Normal.z			 = viewProjectionMatrix.m[2][3] + viewProjectionMatrix.m[2][2];
			Planes[FrustumPlane::FRUSTUM_NEAR].DistanceToOrigin  = viewProjectionMatrix.m[3][3] + viewProjectionMatrix.m[3][2];

			// Normalize all plane normals
			for (uint32 i = 0; i < 6; ++i)
			{
				Planes[i].Normalize();
			}
		}

		bool Frustum::IntersectsSphere(Vec3 sphereCenter, float sphereRadius)
		{
			for (uint32 i = 0; i < 6; ++i)
			{
				if (sphereCenter.Dot(Planes[i].Normal) + Planes[i].DistanceToOrigin + sphereRadius <= 0)
				{
					return false;
				}
			}

			return true;
		}

		bool Frustum::IntersectsAABB(Vec3 aabbPosition, AABB aabb)
		{
			Vec3 box[] = { aabb.GetMinimum(), aabb.GetMaximum() };

			for (uint32 i = 0; i < 6; ++i)
			{
				const FrustumPlane &p = Planes[i];

				const int px = static_cast<int>(p.Normal.x > 0.0f);
				const int py = static_cast<int>(p.Normal.y > 0.0f);
				const int pz = static_cast<int>(p.Normal.z > 0.0f);

				const float dp =
					(p.Normal.x * box[px].x) +
					(p.Normal.y * box[py].y) +
					(p.Normal.y * box[pz].z);

				// Doesn't intersect if it is behind the plane
				if (dp < -p.DistanceToOrigin) { return false; }
			}

			return true;
		}

		FrustumPlane::FrustumPlane()
		{
		}

		FrustumPlane::~FrustumPlane()
		{
		}

		void FrustumPlane::Normalize()
		{
			float scale = 1.0 / Normal.length();
			Normal.x *= scale;
			Normal.y *= scale;
			Normal.z *= scale;
			DistanceToOrigin *= scale;
		}
	};
};
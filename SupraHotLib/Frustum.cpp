#include "Frustum.h"
#include "Mat4.h"
#include "AABB.h"
#include <algorithm>

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

		bool Frustum::IntersectsAABB(AABB aabb, Vec3 position, Vec3 scale)
		{
			Vec3 min = aabb.GetMinimum() * scale;
			Vec3 max = aabb.GetMaximum() * scale;

			

			// check box outside/inside of frustum
			for (uint32 i = 0; i<6; i++)
			{
				int out = 0;
				//out += (Planes[i].Normal.Dot(Vec3(min.x, min.y, min.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(min.x, min.y, min.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(max.x, min.y, min.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(max.x, min.y, min.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(min.x, max.y, min.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(min.x, max.y, min.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(max.x, max.y, min.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(max.x, max.y, min.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(min.x, min.y, max.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(min.x, min.y, max.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(max.x, min.y, max.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(max.x, min.y, max.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(min.x, max.y, max.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(min.x, max.y, max.z), 0) ? 1 : 0;

				//out += (Planes[i].Normal.Dot(Vec3(max.x, max.y, max.z)) + Planes[i].DistanceToOrigin) < 0.0 ? 1 : 0;
				out += !IntersectsSphere(position + Vec3(max.x, max.y, max.z), 0) ? 1 : 0;

				if (out == 8) return false;
			}

			return true;


			/*
			
			// Indexed for the 'index trick' later
			vec3 box[] = { b.min, b.max };

			// We have 6 planes defining the frustum
			static const int NUM_PLANES = 6;
			const plane3 *planes[NUM_PLANES] =
			{ &f.n, &f.l, &f.r, &f.b, &f.t, &f.f };

			// We only need to do 6 point-plane tests
			for (int i = 0; i < NUM_PLANES; ++i)
			{
				// This is the current plane
				const plane3 &p = *planes[i];

				// p-vertex selection (with the index trick)
				// According to the plane normal we can know the
				// indices of the positive vertex
				const int px = static_cast<int>(p.a > 0.0f);
				const int py = static_cast<int>(p.b > 0.0f);
				const int pz = static_cast<int>(p.c > 0.0f);

				// Dot product
				// project p-vertex on plane normal
				// (How far is p-vertex from the origin)
				const float dp =
					(p.a*box[px].x) +
					(p.b*box[py].y) +
					(p.c*box[pz].z);

				// Doesn't intersect if it is behind the plane
				if (dp < -p.d) { return false; }
			}
			return true;
			*/
		}

		FrustumPlane::FrustumPlane()
		{
		}

		FrustumPlane::~FrustumPlane()
		{
		}

		void FrustumPlane::Normalize()
		{
			float scale = 1.0f / Normal.length();
			Normal.x *= scale;
			Normal.y *= scale;
			Normal.z *= scale;
			DistanceToOrigin *= scale;
		}
	};
};
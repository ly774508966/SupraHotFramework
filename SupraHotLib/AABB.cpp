#include "AABB.h"
#include "Mat4.h"

namespace SupraHot
{
	namespace Graphics
	{
		AABB::AABB()
		{
		}

		AABB::AABB(Vec3 minimum, Vec3 maximum)
		{
			this->Minimum = minimum;
			this->Maximum = maximum;
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

		AABB AABB::Transform(Mat4 transformationMatrix)
		{
			float AMin[3], AMax[3];
			float Min[3], Max[3];

			float a, b;
			int32 i, j;

			// Copy box A into min and max array.
			AMin[0] = Minimum.x;
			AMin[1] = Minimum.y;
			AMin[2] = Minimum.z;

			AMax[0] = Maximum.x;
			AMax[1] = Maximum.y;
			AMax[2] = Maximum.z;

			// Begin at T.
			Vec3 translation = transformationMatrix.GetTranslationVector();
			Min[0] = Max[0] = translation.x;
			Min[1] = Max[1] = translation.y;
			Min[2] = Max[2] = translation.z;

			// Find extreme points by considering product of 
			// min and max with each component of M.
			for( j=0; j<3; j++ )
			{
				for( i=0; i<3; i++ )
				{
					a = transformationMatrix.m[i][j] * AMin[i];
					b = transformationMatrix.m[i][j] * AMax[i];

					if( a < b )
					{
						Min[j] += a;
						Max[j] += b;
					}
					else
					{
						Min[j] += b;
						Max[j] += a;
					}
				}
			}

			return AABB(Vec3(Min[0], Min[1], Min[2]), Vec3(Max[0], Max[1], Max[2]));

		}
	};
};
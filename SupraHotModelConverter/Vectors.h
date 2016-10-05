#pragma once

namespace SupraHot
{
	namespace Math
	{
		struct Vec2
		{
			float x;
			float y;

			Vec2() : x(0), y(0)
			{	
			}

			Vec2(float x, float y) : x(x), y(y)
			{
			}
		};

		struct Vec3
		{
			float x;
			float y;
			float z;

			Vec3() : x(0), y(0), z(0)
			{
			}

			Vec3(float x, float y, float z) : x(x), y(y), z(z)
			{
			}
		};
	};
};
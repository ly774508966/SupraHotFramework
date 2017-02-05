#pragma once

namespace SupraHot
{
	namespace Math
	{
		struct Vec4
		{
			float x, y, z, w;
			Vec4();
			Vec4(float x, float y, float z, float w);
			float Dot(const Vec4& v);
			~Vec4();
		};
	};
};


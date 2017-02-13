#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Math
	{
		class Vec3;
		struct Vec4;

		struct Vec2
		{
			float x, y;
			Vec2();
			Vec2(float x, float y);
			~Vec2();

			// --
			// Swizzles

			// vec2 swizzles
			Vec2 xx();
			Vec2 xy();
			Vec2 yx();
			Vec2 yy();

			// vec3 swizzles
			Vec3 xxx();
			Vec3 yxx();
			Vec3 xyx();
			Vec3 yyx();
			Vec3 xxy();
			Vec3 yxy();
			Vec3 xyy();
			Vec3 yyy();

			// vec4 swizzles
			Vec4 xxxx();
			Vec4 yxxx();
			Vec4 xyxx();
			Vec4 yyxx();
			Vec4 xxyx();
			Vec4 yxyx();
			Vec4 xyyx();
			Vec4 yyyx();
			Vec4 xxxy();
			Vec4 yxxy();
			Vec4 xyxy();
			Vec4 yyxy();
			Vec4 xxyy();
			Vec4 yxyy();
			Vec4 xyyy();
			Vec4 yyyy();
		};
	};
};

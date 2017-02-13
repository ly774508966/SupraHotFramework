#include "Vec2.h"
#include "Vec4.h"
#include "Vec3.h"

namespace SupraHot
{
	namespace Math
	{
		Vec2::Vec2()
		{
			this->x = 0;
			this->y = 0;
		}

		Vec2::Vec2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		Vec2::~Vec2()
		{
		}

		// --
		// Swizzles

		// vec2 swizzles
		Vec2 Vec2::xx() { return Vec2(x, x); }
		Vec2 Vec2::xy() { return Vec2(x, y); }
		Vec2 Vec2::yx() { return Vec2(y, x); }
		Vec2 Vec2::yy() { return Vec2(y, y); }

		// vec3 swizzles
		Vec3 Vec2::xxx(){ return Vec3(x, x, x); }
		Vec3 Vec2::yxx(){ return Vec3(y, x, x); }
		Vec3 Vec2::xyx(){ return Vec3(x, y, x); }
		Vec3 Vec2::yyx(){ return Vec3(y, y, x); }
		Vec3 Vec2::xxy(){ return Vec3(x, x, y); }
		Vec3 Vec2::yxy(){ return Vec3(y, x, y); }
		Vec3 Vec2::xyy(){ return Vec3(x, y, y); }
		Vec3 Vec2::yyy(){ return Vec3(y, y, y); }

		// vec4 swizzles
		Vec4 Vec2::xxxx(){ return Vec4(x, x, x, x); }
		Vec4 Vec2::yxxx(){ return Vec4(y, x, x, x); }
		Vec4 Vec2::xyxx(){ return Vec4(x, y, x, x); }
		Vec4 Vec2::yyxx(){ return Vec4(y, y, x, x); }
		Vec4 Vec2::xxyx(){ return Vec4(x, x, y, x); }
		Vec4 Vec2::yxyx(){ return Vec4(y, x, y, x); }
		Vec4 Vec2::xyyx(){ return Vec4(x, y, y, x); }
		Vec4 Vec2::yyyx(){ return Vec4(y, y, y, x); }
		Vec4 Vec2::xxxy(){ return Vec4(x, x, x, y); }
		Vec4 Vec2::yxxy(){ return Vec4(y, x, x, y); }
		Vec4 Vec2::xyxy(){ return Vec4(x, y, x, y); }
		Vec4 Vec2::yyxy(){ return Vec4(y, y, x, y); }
		Vec4 Vec2::xxyy(){ return Vec4(x, x, y, y); }
		Vec4 Vec2::yxyy(){ return Vec4(y, x, y, y); }
		Vec4 Vec2::xyyy(){ return Vec4(x, y, y, y); }
		Vec4 Vec2::yyyy(){ return Vec4(y, y, y, y); }

	}
};

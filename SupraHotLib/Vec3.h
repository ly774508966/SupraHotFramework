#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Math
	{
		struct Vec2;
		struct Vec4;

		class Vec3
		{
		public:
			float x, y, z = 0;

			Vec3();
			Vec3(float x, float y, float z);
			~Vec3();

			void print();

			float length() const;
			float lengthSqr() const;

			Vec3 operator-() const;

			Vec3 operator+(const Vec3& v) const;
			Vec3 operator-(const Vec3& v) const;
			Vec3 operator*(float s) const;
			Vec3 operator*(const Vec3& v) const;
			Vec3 operator/(float s) const;

			Vec3& operator+=(const Vec3& u);
			Vec3& operator-=(const Vec3& u);
			Vec3& operator*=(float scalar);
			Vec3& operator/=(float scalar);
			bool operator==(const Vec3 u);
			bool operator!=(const Vec3 u);

			Vec3 Normalized() const;
			void Normalize();

			float Dot(const Vec3& v) const;
			Vec3 Cross(const Vec3& v) const;

			Vec3 Negated() const;
			void Negate();

			Vec3 Reflect(Vec3 around) const;
			Vec3 Lerp(Vec3 target, float dt) const;
			Vec3 Slerp(const Vec3& axis, float angle);
			float AngleBetween(Vec3 u) const;
			
			float AngleDirection(Vec3& forward, Vec3& direction, Vec3& up);


			// --
			// Swizzles

			// vec2 swizzles
			Vec2 xx();
			Vec2 yx();
			Vec2 zx();
			Vec2 xy();
			Vec2 yy();
			Vec2 zy();
			Vec2 xz();
			Vec2 yz();
			Vec2 zz();

			// vec3 swizzles
			Vec3 xxx();
			Vec3 yxx();
			Vec3 zxx();
			Vec3 xyx();
			Vec3 yyx();
			Vec3 zyx();
			Vec3 xzx();
			Vec3 yzx();
			Vec3 zzx();
			Vec3 xxy();
			Vec3 yxy();
			Vec3 zxy();
			Vec3 xyy();
			Vec3 yyy();
			Vec3 zyy();
			Vec3 xzy();
			Vec3 yzy();
			Vec3 zzy();
			Vec3 xxz();
			Vec3 yxz();
			Vec3 zxz();
			Vec3 xyz();
			Vec3 yyz();
			Vec3 zyz();
			Vec3 xzz();
			Vec3 yzz();
			Vec3 zzz();

			// vec4 swizzles
			Vec4 xxxx();
			Vec4 yxxx();
			Vec4 zxxx();
			Vec4 xyxx();
			Vec4 yyxx();
			Vec4 zyxx();
			Vec4 xzxx();
			Vec4 yzxx();
			Vec4 zzxx();
			Vec4 xxyx();
			Vec4 yxyx();
			Vec4 zxyx();
			Vec4 xyyx();
			Vec4 yyyx();
			Vec4 zyyx();
			Vec4 xzyx();
			Vec4 yzyx();
			Vec4 zzyx();
			Vec4 xxzx();
			Vec4 yxzx();
			Vec4 zxzx();
			Vec4 xyzx();
			Vec4 yyzx();
			Vec4 zyzx();
			Vec4 xzzx();
			Vec4 yzzx();
			Vec4 zzzx();
			Vec4 xxxy();
			Vec4 yxxy();
			Vec4 zxxy();
			Vec4 xyxy();
			Vec4 yyxy();
			Vec4 zyxy();
			Vec4 xzxy();
			Vec4 yzxy();
			Vec4 zzxy();
			Vec4 xxyy();
			Vec4 yxyy();
			Vec4 zxyy();
			Vec4 xyyy();
			Vec4 yyyy();
			Vec4 zyyy();
			Vec4 xzyy();
			Vec4 yzyy();
			Vec4 zzyy();
			Vec4 xxzy();
			Vec4 yxzy();
			Vec4 zxzy();
			Vec4 xyzy();
			Vec4 yyzy();
			Vec4 zyzy();
			Vec4 xzzy();
			Vec4 yzzy();
			Vec4 zzzy();
			Vec4 xxxz();
			Vec4 yxxz();
			Vec4 zxxz();
			Vec4 xyxz();
			Vec4 yyxz();
			Vec4 zyxz();
			Vec4 xzxz();
			Vec4 yzxz();
			Vec4 zzxz();
			Vec4 xxyz();
			Vec4 yxyz();
			Vec4 zxyz();
			Vec4 xyyz();
			Vec4 yyyz();
			Vec4 zyyz();
			Vec4 xzyz();
			Vec4 yzyz();
			Vec4 zzyz();
			Vec4 xxzz();
			Vec4 yxzz();
			Vec4 zxzz();
			Vec4 xyzz();
			Vec4 yyzz();
			Vec4 zyzz();
			Vec4 xzzz();
			Vec4 yzzz();
			Vec4 zzzz();

		};

	}
};

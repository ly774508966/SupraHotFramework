#pragma once

namespace SupraHot
{
	namespace Math
	{
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
			Vec3 operator/(float s) const;

			Vec3& operator+=(const Vec3& u);
			Vec3& operator-=(const Vec3& u);
			Vec3& operator*=(float scalar);
			Vec3& operator/=(float scalar);
			bool operator==(const Vec3 u);
			bool operator!=(const Vec3 u);

			Vec3 normalized() const;
			void normalize();

			float dot(const Vec3& v) const;
			Vec3 cross(const Vec3& v) const;

			Vec3 negated() const;
			void negate();

			Vec3 reflect(Vec3 around) const;
			Vec3 lerp(Vec3 target, float dt) const;
			Vec3 slerp(const Vec3& axis, float angle);
			float angleBetween(Vec3 u) const;
		};

	}
};

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
		};

	}
};

#include "Vec3.h"
#include "MathConstants.h"
#include "Platform.h"

namespace SupraHot
{
	namespace Math
	{
		Vec3::Vec3()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		Vec3::Vec3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vec3::~Vec3() {}

		Vec3 Vec3::operator-() const
		{
			return Vec3(-x, -y, -z);
		}

		Vec3 Vec3::operator+(const Vec3& v) const
		{
			return Vec3(x + v.x, y + v.y, z + v.z);
		}

		Vec3 Vec3::operator-(const Vec3& v) const
		{
			return Vec3(x - v.x, y - v.y, z - v.z);
		}

		Vec3 Vec3::operator*(float s) const
		{
			return Vec3(x * s, y * s, z * s);
		}

		Vec3 Vec3::operator/(float s) const
		{
			float invS = 1.0f / s;
			return Vec3(x * invS, y * invS, z * invS);
		}


		Vec3& Vec3::operator+=(const Vec3& u)
		{
			x += u.x;
			y += u.y;
			z += u.z;

			return *this;
		}

		Vec3& Vec3::operator-=(const Vec3& u)
		{
			x -= u.x;
			y -= u.y;
			z -= u.z;

			return *this;
		}

		Vec3& Vec3::operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		Vec3& Vec3::operator/=(float scalar)
		{
			float inversedScalar = 1.0f / scalar;
			x *= inversedScalar;
			y *= inversedScalar;
			z *= inversedScalar;

			return *this;
		}

		bool Vec3::operator==(const Vec3 u)
		{
			return ((*this - u).length() == 0);
		}

		bool Vec3::operator!=(const Vec3 u)
		{
			return ((*this - u).length() != 0);
		}

		void Vec3::print()
		{
			printf("[%f %f %f]", x, y, z);
		}

		float Vec3::lengthSqr() const
		{
			return x*x + y*y + z*z;
		}

		float Vec3::length() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		void Vec3::normalize()
		{
			float len = 1.0f / this->length();
			this->x *= len;
			this->y *= len;
			this->z *= len;
		}

		Vec3 Vec3::normalized() const
		{
			float len = 1.0f / this->length();
			return Vec3(x * len, y * len, z * len);
		}

		float Vec3::dot(const Vec3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		Vec3 Vec3::cross(const Vec3& v) const
		{
			return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}

		Vec3 Vec3::negated() const
		{
			return Vec3(-x, -y, -z);
		}

		void Vec3::negate()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		Vec3 Vec3::reflect(Vec3 around) const
		{
			Vec3 normal = around.normalized();
			return normal * -2 * dot(normal) + (*this);
		}

		Vec3 Vec3::lerp(Vec3 target, float dt) const
		{
			return (*this) + (target - (*this))  * dt;
		}

		Vec3 Vec3::slerp(const Vec3& axis, float angle)
		{
			float angleRad = ToRadians(angle);
			return (*this) * cos(angleRad) + (axis * (*this).dot(axis) * (1 - cos(angleRad))) + (axis.cross((*this)) * sin(angleRad));
		}

		float Vec3::angleBetween(Vec3 u) const
		{
			return static_cast<float>(acos((dot(u) / (length() * u.length()))) / M_PI * 180);
		}
	}
};

#include "Vec3.h"
#include "MathConstants.h"
#include "Platform.h"
#include "Vec2.h"
#include "Vec4.h"

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

		Vec3 Vec3::operator*(const Vec3& v) const
		{
			return Vec3(x * v.x, y * v.y, z * v.z);
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
			SHF_PRINTF("[%f %f %f]\n", x, y, z);
		}

		float Vec3::lengthSqr() const
		{
			return x*x + y*y + z*z;
		}

		float Vec3::length() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		void Vec3::Normalize()
		{
			float len = 1.0f / this->length();
			this->x *= len;
			this->y *= len;
			this->z *= len;
		}

		Vec3 Vec3::Normalized() const
		{
			float len = 1.0f / this->length();
			return Vec3(x * len, y * len, z * len);
		}

		float Vec3::Dot(const Vec3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		Vec3 Vec3::Cross(const Vec3& v) const
		{
			return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}

		Vec3 Vec3::Negated() const
		{
			return Vec3(-x, -y, -z);
		}

		void Vec3::Negate()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		Vec3 Vec3::Reflect(Vec3 around) const
		{
			Vec3 normal = around.Normalized();
			return normal * -2 * Dot(normal) + (*this);
		}

		Vec3 Vec3::Lerp(Vec3 target, float dt) const
		{
			return (*this) + (target - (*this))  * dt;
		}

		Vec3 Vec3::Slerp(const Vec3& axis, float angle)
		{
			float angleRad = ToRadians(angle);
			return (*this) * cos(angleRad) + (axis * (*this).Dot(axis) * (1 - cos(angleRad))) + (axis.Cross((*this)) * sin(angleRad));
		}

		float Vec3::AngleBetween(Vec3 u) const
		{
			return static_cast<float>(acos((Dot(u) / (length() * u.length()))) / M_PI * 180);
		}

		float Vec3::AngleDirection(Vec3& forward, Vec3& direction, Vec3& up)
		{
			Vec3 perp = forward.Cross(direction);
			float dir = perp.Dot(up);

			if (dir > 0.0) 
			{
				return 1.0;
			}

			if (dir < 0.0) 
			{
				return -1.0;
			}
			
			return 0.0;
		}

		// --
		// Swizzles

		// vec2 swizzles
		Vec2 Vec3::xx(){ return Vec2(x, x); }
		Vec2 Vec3::yx(){ return Vec2(y, x); }
		Vec2 Vec3::zx(){ return Vec2(z, x); }
		Vec2 Vec3::xy(){ return Vec2(x, y); }
		Vec2 Vec3::yy(){ return Vec2(y, y); }
		Vec2 Vec3::zy(){ return Vec2(z, y); }
		Vec2 Vec3::xz(){ return Vec2(x, z); }
		Vec2 Vec3::yz(){ return Vec2(y, z); }
		Vec2 Vec3::zz(){ return Vec2(z, z); }

		// vec3 swizzles
		Vec3 Vec3::xxx(){ return Vec3(x, x, x); }
		Vec3 Vec3::yxx(){ return Vec3(y, x, x); }
		Vec3 Vec3::zxx(){ return Vec3(z, x, x); }
		Vec3 Vec3::xyx(){ return Vec3(x, y, x); }
		Vec3 Vec3::yyx(){ return Vec3(y, y, x); }
		Vec3 Vec3::zyx(){ return Vec3(z, y, x); }
		Vec3 Vec3::xzx(){ return Vec3(x, z, x); }
		Vec3 Vec3::yzx(){ return Vec3(y, z, x); }
		Vec3 Vec3::zzx(){ return Vec3(z, z, x); }
		Vec3 Vec3::xxy(){ return Vec3(x, x, y); }
		Vec3 Vec3::yxy(){ return Vec3(y, x, y); }
		Vec3 Vec3::zxy(){ return Vec3(z, x, y); }
		Vec3 Vec3::xyy(){ return Vec3(x, y, y); }
		Vec3 Vec3::yyy(){ return Vec3(y, y, y); }
		Vec3 Vec3::zyy(){ return Vec3(z, y, y); }
		Vec3 Vec3::xzy(){ return Vec3(x, z, y); }
		Vec3 Vec3::yzy(){ return Vec3(y, z, y); }
		Vec3 Vec3::zzy(){ return Vec3(z, z, y); }
		Vec3 Vec3::xxz(){ return Vec3(x, x, z); }
		Vec3 Vec3::yxz(){ return Vec3(y, x, z); }
		Vec3 Vec3::zxz(){ return Vec3(z, x, z); }
		Vec3 Vec3::xyz(){ return Vec3(x, y, z); }
		Vec3 Vec3::yyz(){ return Vec3(y, y, z); }
		Vec3 Vec3::zyz(){ return Vec3(z, y, z); }
		Vec3 Vec3::xzz(){ return Vec3(x, z, z); }
		Vec3 Vec3::yzz(){ return Vec3(y, z, z); }
		Vec3 Vec3::zzz(){ return Vec3(z, z, z); }

		// vec4 swizzles
		Vec4 Vec3::xxxx(){ return Vec4(x, x, x, x); }
		Vec4 Vec3::yxxx(){ return Vec4(y, x, x, x); }
		Vec4 Vec3::zxxx(){ return Vec4(z, x, x, x); }
		Vec4 Vec3::xyxx(){ return Vec4(x, y, x, x); }
		Vec4 Vec3::yyxx(){ return Vec4(y, y, x, x); }
		Vec4 Vec3::zyxx(){ return Vec4(z, y, x, x); }
		Vec4 Vec3::xzxx(){ return Vec4(x, z, x, x); }
		Vec4 Vec3::yzxx(){ return Vec4(y, z, x, x); }
		Vec4 Vec3::zzxx(){ return Vec4(z, z, x, x); }
		Vec4 Vec3::xxyx(){ return Vec4(x, x, y, x); }
		Vec4 Vec3::yxyx(){ return Vec4(y, x, y, x); }
		Vec4 Vec3::zxyx(){ return Vec4(z, x, y, x); }
		Vec4 Vec3::xyyx(){ return Vec4(x, y, y, x); }
		Vec4 Vec3::yyyx(){ return Vec4(y, y, y, x); }
		Vec4 Vec3::zyyx(){ return Vec4(z, y, y, x); }
		Vec4 Vec3::xzyx(){ return Vec4(x, z, y, x); }
		Vec4 Vec3::yzyx(){ return Vec4(y, z, y, x); }
		Vec4 Vec3::zzyx(){ return Vec4(z, z, y, x); }
		Vec4 Vec3::xxzx(){ return Vec4(x, x, z, x); }
		Vec4 Vec3::yxzx(){ return Vec4(y, x, z, x); }
		Vec4 Vec3::zxzx(){ return Vec4(z, x, z, x); }
		Vec4 Vec3::xyzx(){ return Vec4(x, y, z, x); }
		Vec4 Vec3::yyzx(){ return Vec4(y, y, z, x); }
		Vec4 Vec3::zyzx(){ return Vec4(z, y, z, x); }
		Vec4 Vec3::xzzx(){ return Vec4(x, z, z, x); }
		Vec4 Vec3::yzzx(){ return Vec4(y, z, z, x); }
		Vec4 Vec3::zzzx(){ return Vec4(z, z, z, x); }
		Vec4 Vec3::xxxy(){ return Vec4(x, x, x, y); }
		Vec4 Vec3::yxxy(){ return Vec4(y, x, x, y); }
		Vec4 Vec3::zxxy(){ return Vec4(z, x, x, y); }
		Vec4 Vec3::xyxy(){ return Vec4(x, y, x, y); }
		Vec4 Vec3::yyxy(){ return Vec4(y, y, x, y); }
		Vec4 Vec3::zyxy(){ return Vec4(z, y, x, y); }
		Vec4 Vec3::xzxy(){ return Vec4(x, z, x, y); }
		Vec4 Vec3::yzxy(){ return Vec4(y, z, x, y); }
		Vec4 Vec3::zzxy(){ return Vec4(z, z, x, y); }
		Vec4 Vec3::xxyy(){ return Vec4(x, x, y, y); }
		Vec4 Vec3::yxyy(){ return Vec4(y, x, y, y); }
		Vec4 Vec3::zxyy(){ return Vec4(z, x, y, y); }
		Vec4 Vec3::xyyy(){ return Vec4(x, y, y, y); }
		Vec4 Vec3::yyyy(){ return Vec4(y, y, y, y); }
		Vec4 Vec3::zyyy(){ return Vec4(z, y, y, y); }
		Vec4 Vec3::xzyy(){ return Vec4(x, z, y, y); }
		Vec4 Vec3::yzyy(){ return Vec4(y, z, y, y); }
		Vec4 Vec3::zzyy(){ return Vec4(z, z, y, y); }
		Vec4 Vec3::xxzy(){ return Vec4(x, x, z, y); }
		Vec4 Vec3::yxzy(){ return Vec4(y, x, z, y); }
		Vec4 Vec3::zxzy(){ return Vec4(z, x, z, y); }
		Vec4 Vec3::xyzy(){ return Vec4(x, y, z, y); }
		Vec4 Vec3::yyzy(){ return Vec4(y, y, z, y); }
		Vec4 Vec3::zyzy(){ return Vec4(z, y, z, y); }
		Vec4 Vec3::xzzy(){ return Vec4(x, z, z, y); }
		Vec4 Vec3::yzzy(){ return Vec4(y, z, z, y); }
		Vec4 Vec3::zzzy(){ return Vec4(z, z, z, y); }
		Vec4 Vec3::xxxz(){ return Vec4(x, x, x, z); }
		Vec4 Vec3::yxxz(){ return Vec4(y, x, x, z); }
		Vec4 Vec3::zxxz(){ return Vec4(z, x, x, z); }
		Vec4 Vec3::xyxz(){ return Vec4(x, y, x, z); }
		Vec4 Vec3::yyxz(){ return Vec4(y, y, x, z); }
		Vec4 Vec3::zyxz(){ return Vec4(z, y, x, z); }
		Vec4 Vec3::xzxz(){ return Vec4(x, z, x, z); }
		Vec4 Vec3::yzxz(){ return Vec4(y, z, x, z); }
		Vec4 Vec3::zzxz(){ return Vec4(z, z, x, z); }
		Vec4 Vec3::xxyz(){ return Vec4(x, x, y, z); }
		Vec4 Vec3::yxyz(){ return Vec4(y, x, y, z); }
		Vec4 Vec3::zxyz(){ return Vec4(z, x, y, z); }
		Vec4 Vec3::xyyz(){ return Vec4(x, y, y, z); }
		Vec4 Vec3::yyyz(){ return Vec4(y, y, y, z); }
		Vec4 Vec3::zyyz(){ return Vec4(z, y, y, z); }
		Vec4 Vec3::xzyz(){ return Vec4(x, z, y, z); }
		Vec4 Vec3::yzyz(){ return Vec4(y, z, y, z); }
		Vec4 Vec3::zzyz(){ return Vec4(z, z, y, z); }
		Vec4 Vec3::xxzz(){ return Vec4(x, x, z, z); }
		Vec4 Vec3::yxzz(){ return Vec4(y, x, z, z); }
		Vec4 Vec3::zxzz(){ return Vec4(z, x, z, z); }
		Vec4 Vec3::xyzz(){ return Vec4(x, y, z, z); }
		Vec4 Vec3::yyzz(){ return Vec4(y, y, z, z); }
		Vec4 Vec3::zyzz(){ return Vec4(z, y, z, z); }
		Vec4 Vec3::xzzz(){ return Vec4(x, z, z, z); }
		Vec4 Vec3::yzzz(){ return Vec4(y, z, z, z); }
		Vec4 Vec3::zzzz(){ return Vec4(z, z, z, z); }
	}
};

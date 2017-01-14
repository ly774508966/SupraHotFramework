#include "Quat4.h"
#include "MathConstants.h"
#include <cmath>

namespace SupraHot
{
	namespace Math
	{
		Quat4::Quat4()
		{
			v = Vec3(0, 0, 0);
			w = 1;
		}

		Quat4::Quat4(const Vec3& n, float a)
		{
			a = ToRadians(a);
			float sinHalfAngle = sinf(a / 2);
			float cosHalfAngle = cosf(a / 2);

			v.x = n.x * sinHalfAngle;
			v.y = n.y * sinHalfAngle;
			v.z = n.z * sinHalfAngle;
			w = cosHalfAngle;
		}


		Quat4::~Quat4()
		{

		}

		Quat4 Quat4::Inversed() const
		{
			Quat4 q;
			q.w = w;
			q.v = -v;
			return q;
		}

		Quat4 Quat4::operator*(const Quat4& q) const
		{
			Quat4 result;
			result.w = w*q.w - v.Dot(q.v);
			result.v = v*q.w + q.v*w + v.Cross(q.v);
			return result;
		}

		Vec3 Quat4::operator*(const Vec3& V) const
		{
			Quat4 p;
			p.w = 0;
			p.v = V;
			Vec3 vcV = v.Cross(V);
			return V + vcV*(2 * w) + v.Cross(vcV) * 2;
		}

		Quat4 Quat4::operator^(float t) const
		{
			float a;
			Vec3 n;
			ToAxisAngle(n, a);
			float at = a*t;
			return Quat4(n, at);
		}

		Quat4 Quat4::Slerp(const Quat4& other, float t) const
		{
			const Quat4& q = *this;
			Quat4 r = other;

			float flCosOmega = w*r.w + r.v.Dot(v);
			if (flCosOmega < 0)
			{
				r.w = -r.w;
				r.v = -r.v;
				flCosOmega = -flCosOmega;
			}

			float k0, k1;
			if (flCosOmega > 0.9999f)
			{
				k0 = 1 - t;
				k1 = t;
			}
			else
			{
				float flSinOmega = sqrtf(1 - flCosOmega*flCosOmega);
				float flOmega = atan2f(flSinOmega, flCosOmega);
				float flOneOverSinOmega = 1 / flSinOmega;
				k0 = sinf((1 - t)*flOmega) * flOneOverSinOmega;
				k1 = sinf(t*flOmega) * flOneOverSinOmega;
			}

			// Interpolate
			Quat4 result;
			result.w = q.w * k0 + r.w * k1;
			result.v = q.v * k0 + r.v * k1;

			return result;
		}

		void Quat4::ToAxisAngle(Vec3& vecAxis, float& flAngle) const
		{
			if (v.lengthSqr() < 0.0001f)
				vecAxis = Vec3(1, 0, 0);
			else
				vecAxis = v.Normalized();

			flAngle = acosf(w) * 2.0f;
			flAngle *= 360 / ((float)M_PI * 2);
		}

		void Quat4::Normalize()
		{
			float length = 1.0f / sqrtf(v.lengthSqr() + w * w);
			v *= length;
			w *= length;
		}

		Quat4 Quat4::Normalized() const
		{
			Quat4 q = *this;
			q.Normalize();
			return q;
		}

		Vec3 Quat4::GetAxis()
		{
			return v;
		}

		void Quat4::Conjugate()
		{
			v.Negate();
		}

		Quat4 Quat4::Conjugated() const
		{
			Quat4 q = *this;
			q.Conjugate();
			return q;
		}

		float Quat4::GetAngle()
		{
			return w;
		}

		Vec3 Quat4::GetForward() const
		{
			return (*this) * Vec3(0, 0, -1);
		}

		Vec3 Quat4::GetBack() const
		{
			return (*this) * Vec3(0, 0, 1);
		}

		Vec3 Quat4::GetUp() const
		{
			return (*this) * Vec3(0, 1, 0);
		}

		Vec3 Quat4::GetDown() const
		{
			return (*this) * Vec3(0, -1, 0);
		}

		Vec3 Quat4::GetRight() const
		{
			return (*this) * Vec3(1, 0, 0);
		}

		Vec3 Quat4::GetLeft() const
		{
			return (*this) * Vec3(-1, 0, 0);
		}

		// Euler Angles(zyx ordering)
		Vec3 Quat4::ToEulerAngles()
		{
			Vec3 euler;

			float sqw = w * w;
			float sqx = v.x * v.x;
			float sqy = v.y * v.y;
			float sqz = v.z * v.z;

			float unit = sqx + sqy + sqz + sqw;
			float test = v.x * v.y + v.z * w;

			if (test > 0.499 * unit) 
			{
				euler.x = 2.0f * atan2f(v.x, w);
				euler.y = PI / 2.0f;
				euler.z = 0.0f;
				return euler;
			}

			if (test < -0.499 * unit)
			{ 
				euler.x = -2.0f * atan2f(v.x, w);
				euler.y = -PI / 2.0f;
				euler.z = 0.0f;
				return euler;
			}
			
			euler.x = atan2f(2.0f * v.y * w - 2.0f * v.x * v.z, sqx - sqy - sqz + sqw);
			euler.y = asinf(2.0f * test / unit);
			euler.z = atan2f(2.0f * v.x * w - 2.0f * v.y * v.z, -sqx + sqy - sqz + sqw);

			return euler;
		}

		// Euler Angles(zyx ordering)
		void Quat4::FromEulerAngles(Vec3 eulerAngles)
		{
			/*float c1 = cosf(eulerAngles.y);
			float s1 = sinf(eulerAngles.y);
			float c2 = cosf(eulerAngles.z);
			float s2 = sinf(eulerAngles.z);
			float c3 = cosf(eulerAngles.x);
			float s3 = sinf(eulerAngles.x);
			w = sqrtf(1.0f + c1 * c2 + c1*c3 - s1 * s2 * s3 + c2*c3) / 2.0f;
			float w4 = (4.0f * w);
			v.x = (c2 * s3 + c1 * s3 + s1 * s2 * c3) / w4;
			v.y = (s1 * c2 + s1 * c3 + c1 * s2 * s3) / w4;
			v.z = (-s1 * s3 + c1 * s2 * c3 + s2) / w4;*/


			float c1 = cosf(eulerAngles.y / 2.0f);
			float s1 = sinf(eulerAngles.y / 2.0f);

			float c2 = cosf(eulerAngles.z / 2.0f);
			float s2 = sinf(eulerAngles.z / 2.0f);

			float c3 = cosf(eulerAngles.x / 2.0f);
			float s3 = sinf(eulerAngles.x / 2.0f);

			float c1c2 = c1 * c2;
			float s1s2 = s1 * s2;

			w	= c1c2 * c3 - s1s2 * s3;
			v.x = c1c2 * s3 + s1s2 * c3;
			v.y = s1 * c2 * c3 + c1 * s2 * s3;
			v.z = c1 * s2 * c3 - s1 * c2 * s3;
		}
	};
};
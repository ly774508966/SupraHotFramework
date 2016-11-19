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
	};
};
#pragma once
#include "Vec3.h"

namespace SupraHot
{
	namespace Math
	{
		class Mat4;

		class Quat4 {
		public:
			//members
			Vec3 v;
			float w;

			// contructors
			Quat4();
			Quat4(const Vec3& n, float angleInDegrees);
			~Quat4();

			// methods
			Quat4 Inversed() const;
			Quat4 operator*(const Quat4& q) const;
			Vec3 operator*(const Vec3& p) const;
			Quat4 operator^(float t) const;
			Quat4 Slerp(const Quat4& r, float t) const;
			void ToAxisAngle(Vec3& vecAxis, float& flAngle) const;

			// Euler Angles (zyx ordering)
			Vec3 ToEulerAngles();
			void FromEulerAngles(Vec3 eulerAngles);
			void FromRotationMatrix(Mat4& rotationMatrix);

			void Normalize();
			Quat4 Normalized() const;

			Vec3 GetAxis();
			float GetRotationComponent();
			float GetAngleRadians();
			float GetAngleDegrees();
			
			void Conjugate();
			Quat4 Conjugated() const;

			Vec3 GetForward() const;
			Vec3 GetBack() const;
			Vec3 GetUp() const;
			Vec3 GetDown() const;
			Vec3 GetRight() const;
			Vec3 GetLeft() const;
		};
	};
};
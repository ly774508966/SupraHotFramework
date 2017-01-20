#pragma once
#include "MathConstants.h"
#include "Platform.h"
#include "Vec3.h"

namespace SupraHot
{
	namespace Math
	{
		class Quat4;
		class Mat4
		{
		public:
			// members
			float m[4][4];

			// constructors
			Mat4();
			Mat4(float* floatArray);
			Mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
			Mat4(const Vec3& forward, const Vec3& up, const Vec3& right, const Vec3& position);
			Mat4(const Mat4& i);
			~Mat4();

			// methods
			void Transpose();
			Mat4 Transposed() const;

			void InverseTR();
			Mat4 InversedTR() const;

			void Inverse();
			Mat4 Inversed() const;

			void Identity();

			void Print();

			void SetForwardVector(const Vec3& v);
			void SetRightVector(const Vec3& v);
			void SetUpVector(const Vec3& v);
			void SetTranslationVector(const Vec3& v);
			void SetRotation(const Quat4& q);
			void SetRotation(const float angle, const Vec3& axis);
			void SetScale(const Vec3& scale);

			Vec3 GetForwardVector() const { return Vec3(m[0][0], m[0][1], m[0][2]); }
			Vec3 GetUpVector() const { return Vec3(m[1][0], m[1][1], m[1][2]); }
			Vec3 GetRightVector() const { return Vec3(m[2][0], m[2][1], m[2][2]); }
			Vec3 GetTranslationVector() const{ return Vec3(m[3][0], m[3][1], m[3][2]); }
			Mat4 ToRotationMatrix(Quat4 q);

			Mat4 operator*(const Mat4& M) const;
			Vec3 operator*(const Vec3& v) const;
			Mat4 operator*(float f) const;
			Mat4 operator*=(const Mat4& t);

			// perspective
			static Mat4	ProjectPerspective(float flFOV, float flAspectRatio, float flNear, float flFar);
			static Mat4	ProjectFrustum(float flFOV, float flAspectRatio, float flNear, float flFar);
			static Mat4	ProjectFrustum(float flLeft, float flRight, float flBottom, float flTop, float flNear, float flFar);
			static Mat4	ProjectOrthographic(float flLeft, float flRight, float flBottom, float flTop, float flNear, float flFar);
			static Mat4	ConstructCameraView(const Vec3& vecPosition, const Vec3& vecDirection, const Vec3& vecUp);

			void SetOrthographic(float flLeft, float flRight, float flBottom, float flTop, float flNear, float flFar);

			// rotations
			static Mat4 RotationFromAxisAndAngle(float angle, const Vec3& axis);
			static Mat4 RotationFromQuaternion(const Quat4& q);
		};
	};
};
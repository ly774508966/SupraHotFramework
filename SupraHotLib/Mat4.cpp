#include "Mat4.h"
#include "Quat4.h"

namespace SupraHot
{
	namespace Math
	{
		/***
		*	Layout: Column-Major
		*
		*	The data gets passed column by colum to open gl.
		*
		*	00	10	20	30
		*	01	11	21	31
		*	02	12	22	32
		*	03	13	23	33
		*
		*	float[16] = {00, 01, 02, 03, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33};
		*/

		Mat4::Mat4()
		{
			memset(this, 0, sizeof(Mat4));
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		}

		Mat4::Mat4(float* floatArray)
		{
			memcpy(&m[0][0], floatArray, sizeof(float) * 16);
		}

		Mat4::Mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
			m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
			m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
			m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
		}

		Mat4::Mat4(const Vec3& forward, const Vec3& up, const Vec3& right, const Vec3& position)
		{
			SetForwardVector(forward);
			SetUpVector(up);
			SetRightVector(right);
			SetTranslationVector(position);

			m[0][3] = 0;
			m[1][3] = 0;
			m[2][3] = 0;
			m[3][3] = 1;
		}

		Mat4::Mat4(const Mat4& i)
		{
			m[0][0] = i.m[0][0]; m[1][0] = i.m[1][0]; m[2][0] = i.m[2][0]; m[3][0] = i.m[3][0];
			m[0][1] = i.m[0][1]; m[1][1] = i.m[1][1]; m[2][1] = i.m[2][1]; m[3][1] = i.m[3][1];
			m[0][2] = i.m[0][2]; m[1][2] = i.m[1][2]; m[2][2] = i.m[2][2]; m[3][2] = i.m[3][2];
			m[0][3] = i.m[0][3]; m[1][3] = i.m[1][3]; m[2][3] = i.m[2][3]; m[3][3] = i.m[3][3];
		}

		Mat4::~Mat4(){

		}

		void Mat4::Identity()
		{
			memset(this, 0, sizeof(Mat4));
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		}

		void Mat4::SetForwardVector(const Vec3& v)
		{
			m[0][0] = v.x;
			m[0][1] = v.y;
			m[0][2] = v.z;
		}

		void Mat4::SetRightVector(const Vec3& v)
		{
			m[2][0] = v.x;
			m[2][1] = v.y;
			m[2][2] = v.z;
		}

		void Mat4::SetUpVector(const Vec3& v)
		{
			m[1][0] = v.x;
			m[1][1] = v.y;
			m[1][2] = v.z;
		}

		void Mat4::SetTranslationVector(const Vec3& v)
		{
			m[3][0] = v.x;
			m[3][1] = v.y;
			m[3][2] = v.z;
		}

		void Mat4::Transpose()
		{
			(*this) = Transposed();
		}

		Mat4 Mat4::Transposed() const
		{
			Mat4 r;
			r.m[0][0] = m[0][0]; r.m[1][0] = m[0][1]; r.m[2][0] = m[0][2]; r.m[3][0] = m[0][3];
			r.m[0][1] = m[1][0]; r.m[1][1] = m[1][1]; r.m[2][1] = m[1][2]; r.m[3][1] = m[1][3];
			r.m[0][2] = m[2][0]; r.m[1][2] = m[2][1]; r.m[2][2] = m[2][2]; r.m[3][2] = m[2][3];
			r.m[0][3] = m[3][0]; r.m[1][3] = m[3][1]; r.m[2][3] = m[3][2]; r.m[3][3] = m[3][3];
			return r;
		}

		void Mat4::InverseTR()
		{
			(*this) = InversedTR();
		}

		Mat4 Mat4::InversedTR() const
		{
			Mat4 result;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					result.m[i][j] = m[j][i];
				}
			}

			result.SetTranslationVector(-(result * GetTranslationVector()));
			return result;
		}

		void Mat4::Inverse()
		{
			(*this) = Inversed();
		}


		// Source: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		Mat4 Mat4::Inversed() const
		{
			Mat4 result;

			float det;

			//				0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15
			// float[16] = {00, 01, 02, 03, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33};

			result.m[0][0] = m[1][1] * m[2][2] * m[3][3] -
							 m[1][1] * m[2][3] * m[3][2] -
							 m[2][1] * m[1][2] * m[3][3] +
							 m[2][1] * m[1][3] * m[3][2] +
							 m[3][1] * m[1][2] * m[2][3] -
							 m[3][1] * m[1][3] * m[2][2];

			result.m[1][0] = -m[1][0] * m[2][2] * m[3][3] +
							  m[1][0] * m[2][3] * m[3][2] +
							  m[2][0] * m[1][2] * m[3][3] -
							  m[2][0] * m[1][3] * m[3][2] -
							  m[3][0] * m[1][2] * m[2][3] +
							  m[3][0] * m[1][3] * m[2][2];

			result.m[2][0] = m[1][0] * m[2][1] * m[3][3] -
							 m[1][0] * m[2][3] * m[3][1] -
							 m[2][0] * m[1][1] * m[3][3] +
							 m[2][0] * m[1][3] * m[3][1] +
							 m[3][0] * m[1][1] * m[2][3] -
							 m[3][0] * m[1][3] * m[2][1];

			result.m[3][0] = -m[1][0] * m[2][1] * m[3][2] +
							  m[1][0] * m[2][2] * m[3][1] +
							  m[2][0] * m[1][1] * m[3][2] -
							  m[2][0] * m[1][2] * m[3][1] -
							  m[3][0] * m[1][1] * m[2][2] +
							  m[3][0] * m[1][2] * m[2][1];

			result.m[0][1] = -m[0][1] * m[2][2] * m[3][3] +
							  m[0][1] * m[2][3] * m[3][2] +
							  m[2][1] * m[0][2] * m[3][3] -
							  m[2][1] * m[0][3] * m[3][2] -
							  m[3][1] * m[0][2] * m[2][3] +
							  m[3][1] * m[0][3] * m[2][2];

			result.m[1][1] = m[0][0] * m[2][2] * m[3][3] -
							 m[0][0] * m[2][3] * m[3][2] -
							 m[2][0] * m[0][2] * m[3][3] +
							 m[2][0] * m[0][3] * m[3][2] +
							 m[3][0] * m[0][2] * m[2][3] -
							 m[3][0] * m[0][3] * m[2][2];

			result.m[2][1] = -m[0][0] * m[2][1] * m[3][3] +
							  m[0][0] * m[2][3] * m[3][1] +
							  m[2][0] * m[0][1] * m[3][3] -
							  m[2][0] * m[0][3] * m[3][1] -
							  m[3][0] * m[0][1] * m[2][3] +
							  m[3][0] * m[0][3] * m[2][1];
							  
			result.m[3][1] = m[0][0] * m[2][1] * m[3][2] -
							 m[0][0] * m[2][2] * m[3][1] -
							 m[2][0] * m[0][1] * m[3][2] +
							 m[2][0] * m[0][2] * m[3][1] +
							 m[3][0] * m[0][1] * m[2][2] -
							 m[3][0] * m[0][2] * m[2][1];

			result.m[0][2] = m[0][1] * m[1][2] * m[3][3] -
							 m[0][1] * m[1][3] * m[3][2] -
							 m[1][1] * m[0][2] * m[3][3] +
							 m[1][1] * m[0][3] * m[3][2] +
							 m[3][1] * m[0][2] * m[1][3] -
							 m[3][1] * m[0][3] * m[1][2];

			result.m[1][2] = -m[0][0] * m[1][2] * m[3][3] +
							  m[0][0] * m[1][3] * m[3][2] +
							  m[1][0] * m[0][2] * m[3][3] -
							  m[1][0] * m[0][3] * m[3][2] -
							  m[3][0] * m[0][2] * m[1][3] +
							  m[3][0] * m[0][3] * m[1][2];

			result.m[2][2] = m[0][0] * m[1][1] * m[3][3] -
							 m[0][0] * m[1][3] * m[3][1] -
							 m[1][0] * m[0][1] * m[3][3] +
							 m[1][0] * m[0][3] * m[3][1] +
							 m[3][0] * m[0][1] * m[1][3] -
							 m[3][0] * m[0][3] * m[1][1];

			result.m[3][2] = -m[0][0] * m[1][1] * m[3][2] +
							  m[0][0] * m[1][2] * m[3][1] +
							  m[1][0] * m[0][1] * m[3][2] -
							  m[1][0] * m[0][2] * m[3][1] -
							  m[3][0] * m[0][1] * m[1][2] +
							  m[3][0] * m[0][2] * m[1][1];

			result.m[0][3] = -m[0][1] * m[1][2] * m[2][3] +
							  m[0][1] * m[1][3] * m[2][2] +
							  m[1][1] * m[0][2] * m[2][3] -
							  m[1][1] * m[0][3] * m[2][2] -
							  m[2][1] * m[0][2] * m[1][3] +
							  m[2][1] * m[0][3] * m[1][2];

			result.m[1][3] = m[0][0] * m[1][2] * m[2][3] -
							 m[0][0] * m[1][3] * m[2][2] -
							 m[1][0] * m[0][2] * m[2][3] +
							 m[1][0] * m[0][3] * m[2][2] +
							 m[2][0] * m[0][2] * m[1][3] -
							 m[2][0] * m[0][3] * m[1][2];

			result.m[2][3] = -m[0][0] * m[1][1] * m[2][3] +
							  m[0][0] * m[1][3] * m[2][1] +
							  m[1][0] * m[0][1] * m[2][3] -
							  m[1][0] * m[0][3] * m[2][1] -
							  m[2][0] * m[0][1] * m[1][3] +
							  m[2][0] * m[0][3] * m[1][1];

			result.m[3][3] = m[0][0] * m[1][1] * m[2][2] -
							 m[0][0] * m[1][2] * m[2][1] -
							 m[1][0] * m[0][1] * m[2][2] +
							 m[1][0] * m[0][2] * m[2][1] +
							 m[2][0] * m[0][1] * m[1][2] -
							 m[2][0] * m[0][2] * m[1][1];

			det = m[0][0] * result.m[0][0] + m[0][1] * result.m[1][0] + m[0][2] * result.m[2][0] + m[0][3] * result.m[3][0];

			if (det != 0)
			{
				det = 1.0f / det;

				result.m[0][0] *= det;
				result.m[0][1] *= det;
				result.m[0][2] *= det;
				result.m[0][3] *= det;

				result.m[1][0] *= det;
				result.m[1][1] *= det;
				result.m[1][2] *= det;
				result.m[1][3] *= det;

				result.m[2][0] *= det;
				result.m[2][1] *= det;
				result.m[2][2] *= det;
				result.m[2][3] *= det;

				result.m[3][0] *= det;
				result.m[3][1] *= det;
				result.m[3][2] *= det;
				result.m[3][3] *= det;
			}
			else
			{
				SHF_PRINTF("No determinant found \n");
			}
			
			return result;
		}

		void Mat4::Print()
		{
			printf("%f | %f | %f | %f \n", m[0][0], m[1][0], m[2][0], m[3][0]);
			printf("%f | %f | %f | %f \n", m[0][1], m[1][1], m[2][1], m[3][1]);
			printf("%f | %f | %f | %f \n", m[0][2], m[1][2], m[2][2], m[3][2]);
			printf("%f | %f | %f | %f \n", m[0][3], m[1][3], m[2][3], m[3][3]);
			printf("- - - - - - - - - - \n");
		}

		// Used for vectors which represent a position.
		Vec3 Mat4::operator*(const Vec3& v) const
		{
			// [a b c x][X]
			// [d e f y][Y] = [aX+bY+cZ+x dX+eY+fZ+y gX+hY+iZ+z]
			// [g h i z][Z]
			//          [1]

			Vec3 result;
			result.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
			result.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
			result.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];
			return result;
		}

		Mat4 Mat4::operator*(const Mat4& right) const
		{
			Mat4 result;

			// [a b c d][A B C D]   [aA+bE+cI+dM
			// [e f g h][E F G H] = [eA+fE+gI+hM ...
			// [i j k l][I J K L]
			// [m n o p][M N O P]

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.m[i][j] = m[0][j] * right.m[i][0] + m[1][j] * right.m[i][1] + m[2][j] * right.m[i][2] + m[3][j] * right.m[i][3];
				}
			}

			return result;
		}

		Mat4 Mat4::operator*=(const Mat4& t)
		{
			*this = (*this) * t;
			return *this;
		}

		Mat4 Mat4::operator*(float f) const
		{
			Mat4 result;

			result.m[0][0] = m[0][0] * f; result.m[1][0] = m[1][0] * f; result.m[2][0] = m[2][0] * f; result.m[3][0] = m[3][0] * f;
			result.m[0][1] = m[0][1] * f; result.m[1][1] = m[1][1] * f; result.m[2][1] = m[2][1] * f; result.m[3][1] = m[3][1] * f;
			result.m[0][2] = m[0][2] * f; result.m[1][2] = m[1][2] * f; result.m[2][2] = m[2][2] * f; result.m[3][2] = m[3][2] * f;
			result.m[0][3] = m[0][3] * f; result.m[1][3] = m[1][3] * f; result.m[2][3] = m[2][3] * f; result.m[3][3] = m[3][3] * f;

			return result;
		}

		// perspective
		Mat4 Mat4::ProjectPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
		{
			float tanThetaOver2 = tanf(fieldOfView * static_cast<float>(M_PI) / 360);

			Mat4 m;
			m.Identity();

			m.m[0][0] = 1 / tanThetaOver2;
											m.m[1][1] = aspectRatio / tanThetaOver2;
																						m.m[2][2] = (nearPlane + farPlane) / (nearPlane - farPlane);	m.m[3][2] = 2 * nearPlane * farPlane / (nearPlane - farPlane);
																						m.m[2][3] = -1;													m.m[3][3] = 0;
			return m;
		}

		Mat4 Mat4::ProjectFrustum(float flFOV, float flAspectRatio, float flNear, float flFar) {
			float flRight = flNear * tanf(flFOV * static_cast<float>(M_PI) / 360);
			float flLeft = -flRight;

			float flBottom = flLeft / flAspectRatio;
			float flTop = flRight / flAspectRatio;

			return ProjectFrustum(flLeft, flRight, flBottom, flTop, flNear, flFar);
		}

		Mat4 Mat4::ProjectFrustum(float flLeft, float flRight, float flBottom, float flTop, float flNear, float flFar)
		{
			Mat4 m;

			m.Identity();

			float flXD = flRight - flLeft;
			float flYD = flTop - flBottom;
			float flZD = flFar - flNear;

			m.m[0][0] = (2 * flNear) / flXD;											m.m[2][0] = (flRight + flLeft) / flXD;
												m.m[1][1] = (2 * flNear) / flYD;		m.m[2][1] = (flTop + flBottom) / flYD;
																						m.m[2][2] = -(flFar + flNear) / flZD;		m.m[3][2] = -(2 * flFar * flNear) / flZD;
																						m.m[2][3] = -1;								m.m[3][3] = 0;
			return m;
		}

		Mat4 Mat4::ProjectOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
		{
			Mat4 m;
			m.Identity();

			float deltaX = right - left;
			float deltaY = top - bottom;
			float deltaZ = farPlane - nearPlane;

			m.m[0][0] = 2.0f / deltaX;																m.m[3][0] = -(right + left) / deltaX;
										m.m[1][1] = 2.0f / deltaY;									m.m[3][1] = -(top + bottom) / deltaY;
																	m.m[2][2] = -2.0f / deltaZ;		m.m[3][2] = -(farPlane + nearPlane) / deltaZ;
			return m;
		}


		Mat4 Mat4::ConstructCameraView(const Vec3& position, const Vec3& lookDirection, const Vec3& vecUp)
		{
			Vec3 rightVector = lookDirection.Cross(vecUp).Normalized();
			Vec3 upVector = rightVector.Cross(lookDirection);
			return Mat4(rightVector, upVector, -lookDirection, position).InversedTR();
		}

		Mat4 Mat4::RotationFromAxisAndAngle(float angle, const Vec3& axis)
		{
			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			float c = cosf(ToRadians(angle));
			float s = sinf(ToRadians(angle));
			float t = 1 - c;

			Mat4 result;
			result.m[0][0] = x * x * t + c;		result.m[1][0] = x * y * t - z * s;	result.m[2][0] = x * z * t + y * s;
			result.m[0][1] = y * x * t + z * s;	result.m[1][1] = y * y * t + c;		result.m[2][1] = y * z * t - x * s;
			result.m[0][2] = z * x * t - y * s;	result.m[1][2] = z * y * t + x * s;	result.m[2][2] = z * z * t + c;
			return result;
		}

		Mat4 Mat4::RotationFromQuaternion(const Quat4& q)
		{
			Vec3 axis;
			float angle;
			q.ToAxisAngle(axis, angle);
			return RotationFromAxisAndAngle(angle, axis);
		}

		void Mat4::SetRotation(const Quat4& q)
		{

			Vec3 v;
			float angle = 0;
			q.ToAxisAngle(v, angle);

			float x = v.x;
			float y = v.y;
			float z = v.z;

			float c = cosf(angle*static_cast<float>(M_PI) / 180);
			float s = sinf(angle*static_cast<float>(M_PI) / 180);
			float t = 1 - c;

			m[0][0] = x * x * t + c;	m[1][0] = x * y * t - z *s;	m[2][0] = x * z * t + y *s;
			m[0][1] = y*x*t + z*s;		m[1][1] = y*y*t + c;		m[2][1] = y*z*t - x*s;
			m[0][2] = z*x*t - y*s;		m[1][2] = z*y*t + x*s;		m[2][2] = z*z*t + c;
		}

		void Mat4::SetRotation(const float angle, const Vec3& axis)
		{
			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			float c = cos(angle * (float)M_PI / 180);
			float s = sin(angle * (float)M_PI / 180);
			float t = 1 - c;

			m[0][0] = x*x*t + c;
			m[1][0] = x*y*t - z*s;
			m[2][0] = x*z*t + y*s;

			m[0][1] = y*x*t + z*s;
			m[1][1] = y*y*t + c;
			m[2][1] = y*z*t - x*s;

			m[0][2] = z*x*t - y*s;
			m[1][2] = z*y*t + x*s;
			m[2][2] = z*z*t + c;
		}

		void Mat4::SetScale(const Vec3& scale)
		{
			m[0][0] = scale.x;
			m[1][1] = scale.y;
			m[2][2] = scale.z;
		}

		Vec3 Mat4::GetScaleVector() const
		{
			float sx = Vec3(m[0][0], m[0][1], m[0][2]).length();
			float sy = Vec3(m[1][0], m[1][1], m[1][2]).length();
			float sz = Vec3(m[2][0], m[2][1], m[2][2]).length();


			if (GetDeterminat() < 0)
			{
				sx = -sx;
			}

			return Vec3(sx, sy, sz);
		}

		Mat4 Mat4::ToRotationMatrix(Quat4 q)
		{
			Vec3 v = q.v;
			float w = q.w;

			Vec3 forward = Vec3(2.0f * (v.x * v.z - w * v.y), 2.0f * (v.y * v.z + w * v.x), 1.0f - 2.0f * (v.x * v.x + v.y * v.y));
			Vec3 up = Vec3(2.0f * (v.x * v.y + w * v.z), 1.0f - 2.0f * (v.x * v.x + v.z * v.z), 2.0f * (v.y * v.z - w * v.x));
			Vec3 right = Vec3(1.0f - 2.0f * (v.y * v.y + v.z * v.z), 2.0f * (v.x * v.y - w * v.z), 2.0f * (v.x * v.z + w * v.y));

			Mat4 m;
			m.m[0][0] = right.x;    m.m[1][0] = right.y;    m.m[2][0] = right.z;    m.m[3][0] = 0.0f;
			m.m[0][1] = up.x;       m.m[1][1] = up.y;       m.m[2][1] = up.z;       m.m[3][1] = 0.0f;
			m.m[0][2] = forward.x;  m.m[1][2] = forward.y;  m.m[2][2] = forward.z;  m.m[3][2] = 0.0f;
			m.m[0][3] = 0.0f;       m.m[1][3] = 0.0f;       m.m[2][3] = 0.0f;       m.m[3][3] = 1.0f;

			return m;
		}

		float Mat4::GetDeterminat() const
		{
			Mat4 result;
			float det;

			//				0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15
			// float[16] = {00, 01, 02, 03, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33};

			result.m[0][0] = m[1][1] * m[2][2] * m[3][3] -
				m[1][1] * m[2][3] * m[3][2] -
				m[2][1] * m[1][2] * m[3][3] +
				m[2][1] * m[1][3] * m[3][2] +
				m[3][1] * m[1][2] * m[2][3] -
				m[3][1] * m[1][3] * m[2][2];

			result.m[1][0] = -m[1][0] * m[2][2] * m[3][3] +
				m[1][0] * m[2][3] * m[3][2] +
				m[2][0] * m[1][2] * m[3][3] -
				m[2][0] * m[1][3] * m[3][2] -
				m[3][0] * m[1][2] * m[2][3] +
				m[3][0] * m[1][3] * m[2][2];

			result.m[2][0] = m[1][0] * m[2][1] * m[3][3] -
				m[1][0] * m[2][3] * m[3][1] -
				m[2][0] * m[1][1] * m[3][3] +
				m[2][0] * m[1][3] * m[3][1] +
				m[3][0] * m[1][1] * m[2][3] -
				m[3][0] * m[1][3] * m[2][1];

			result.m[3][0] = -m[1][0] * m[2][1] * m[3][2] +
				m[1][0] * m[2][2] * m[3][1] +
				m[2][0] * m[1][1] * m[3][2] -
				m[2][0] * m[1][2] * m[3][1] -
				m[3][0] * m[1][1] * m[2][2] +
				m[3][0] * m[1][2] * m[2][1];

			result.m[0][1] = -m[0][1] * m[2][2] * m[3][3] +
				m[0][1] * m[2][3] * m[3][2] +
				m[2][1] * m[0][2] * m[3][3] -
				m[2][1] * m[0][3] * m[3][2] -
				m[3][1] * m[0][2] * m[2][3] +
				m[3][1] * m[0][3] * m[2][2];

			result.m[1][1] = m[0][0] * m[2][2] * m[3][3] -
				m[0][0] * m[2][3] * m[3][2] -
				m[2][0] * m[0][2] * m[3][3] +
				m[2][0] * m[0][3] * m[3][2] +
				m[3][0] * m[0][2] * m[2][3] -
				m[3][0] * m[0][3] * m[2][2];

			result.m[2][1] = -m[0][0] * m[2][1] * m[3][3] +
				m[0][0] * m[2][3] * m[3][1] +
				m[2][0] * m[0][1] * m[3][3] -
				m[2][0] * m[0][3] * m[3][1] -
				m[3][0] * m[0][1] * m[2][3] +
				m[3][0] * m[0][3] * m[2][1];

			result.m[3][1] = m[0][0] * m[2][1] * m[3][2] -
				m[0][0] * m[2][2] * m[3][1] -
				m[2][0] * m[0][1] * m[3][2] +
				m[2][0] * m[0][2] * m[3][1] +
				m[3][0] * m[0][1] * m[2][2] -
				m[3][0] * m[0][2] * m[2][1];

			result.m[0][2] = m[0][1] * m[1][2] * m[3][3] -
				m[0][1] * m[1][3] * m[3][2] -
				m[1][1] * m[0][2] * m[3][3] +
				m[1][1] * m[0][3] * m[3][2] +
				m[3][1] * m[0][2] * m[1][3] -
				m[3][1] * m[0][3] * m[1][2];

			result.m[1][2] = -m[0][0] * m[1][2] * m[3][3] +
				m[0][0] * m[1][3] * m[3][2] +
				m[1][0] * m[0][2] * m[3][3] -
				m[1][0] * m[0][3] * m[3][2] -
				m[3][0] * m[0][2] * m[1][3] +
				m[3][0] * m[0][3] * m[1][2];

			result.m[2][2] = m[0][0] * m[1][1] * m[3][3] -
				m[0][0] * m[1][3] * m[3][1] -
				m[1][0] * m[0][1] * m[3][3] +
				m[1][0] * m[0][3] * m[3][1] +
				m[3][0] * m[0][1] * m[1][3] -
				m[3][0] * m[0][3] * m[1][1];

			result.m[3][2] = -m[0][0] * m[1][1] * m[3][2] +
				m[0][0] * m[1][2] * m[3][1] +
				m[1][0] * m[0][1] * m[3][2] -
				m[1][0] * m[0][2] * m[3][1] -
				m[3][0] * m[0][1] * m[1][2] +
				m[3][0] * m[0][2] * m[1][1];

			result.m[0][3] = -m[0][1] * m[1][2] * m[2][3] +
				m[0][1] * m[1][3] * m[2][2] +
				m[1][1] * m[0][2] * m[2][3] -
				m[1][1] * m[0][3] * m[2][2] -
				m[2][1] * m[0][2] * m[1][3] +
				m[2][1] * m[0][3] * m[1][2];

			result.m[1][3] = m[0][0] * m[1][2] * m[2][3] -
				m[0][0] * m[1][3] * m[2][2] -
				m[1][0] * m[0][2] * m[2][3] +
				m[1][0] * m[0][3] * m[2][2] +
				m[2][0] * m[0][2] * m[1][3] -
				m[2][0] * m[0][3] * m[1][2];

			result.m[2][3] = -m[0][0] * m[1][1] * m[2][3] +
				m[0][0] * m[1][3] * m[2][1] +
				m[1][0] * m[0][1] * m[2][3] -
				m[1][0] * m[0][3] * m[2][1] -
				m[2][0] * m[0][1] * m[1][3] +
				m[2][0] * m[0][3] * m[1][1];

			result.m[3][3] = m[0][0] * m[1][1] * m[2][2] -
				m[0][0] * m[1][2] * m[2][1] -
				m[1][0] * m[0][1] * m[2][2] +
				m[1][0] * m[0][2] * m[2][1] +
				m[2][0] * m[0][1] * m[1][2] -
				m[2][0] * m[0][2] * m[1][1];

			det = m[0][0] * result.m[0][0] + m[0][1] * result.m[1][0] + m[0][2] * result.m[2][0] + m[0][3] * result.m[3][0];

			return det;
		}

		void Mat4::SetOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
		{
			Identity();

			float deltaX = right - left;
			float deltaY = top - bottom;
			float deltaZ = farPlane - nearPlane;

			m[0][0] = 2.0f / deltaX;															m[3][0] = -(right + left) / deltaX;
										m[1][1] = 2.0f / deltaY;								m[3][1] = -(top + bottom) / deltaY;
																	m[2][2] = -2.0f / deltaZ;	m[3][2] = -(farPlane + nearPlane) / deltaZ;
		}
	};
};

#include "Transform.h"

namespace SupraHot
{
	namespace Math
	{
		Transform::Transform()
		{
			GlobalScale = Vec3(1, 1, 1);
			LocalScale = Vec3(1, 1, 1);
			LocalRotation = Quat4(Vec3(0, 0, 0), 1);
			GlobalRotation = Quat4(Vec3(0, 0, 0), 1);
		}

		Transform::~Transform()
		{

		}

		Mat4 Transform::GetTransformation()
		{

			Transformation.Identity();
			LocalRotation.Normalize();
			GlobalRotation.Normalize();

			Mat4 localRotationMatrix;
			localRotationMatrix = localRotationMatrix.ToRotationMatrix(LocalRotation);

			Mat4 translationMatrix;
			translationMatrix.SetTranslationVector(Position);

			Mat4 globalRotationMatrix;
			globalRotationMatrix = globalRotationMatrix.ToRotationMatrix(GlobalRotation);

			Mat4 globalScaleMatrix;
			globalScaleMatrix.SetScale(GlobalScale);

			Mat4 localScaleMatrix;
			localScaleMatrix.SetScale(LocalScale);

			Transformation = globalScaleMatrix * globalRotationMatrix * translationMatrix * localRotationMatrix * localScaleMatrix;
			return Transformation;
		}

		void Transform::SetLocalRotation(const Quat4 &rotation)
		{
			LocalRotation = rotation;
		}

		void Transform::SetGlobalRotation(const Quat4 &rotation)
		{
			GlobalRotation = rotation;
		}

		void Transform::SetPosition(const Vec3 &position)
		{
			this->Position = position;
		}

		void Transform::SetGlobalScale(const Vec3 &scale)
		{
			this->GlobalScale = scale;
		}

		void Transform::SetLocalScale(const Vec3 &scale)
		{
			this->LocalScale = scale;
		}

		Quat4* Transform::GetLocalRotation()
		{
			return &LocalRotation;
		}

		Quat4* Transform::GetGlobalRotation()
		{
			return &GlobalRotation;
		}

		Vec3* Transform::GetPosition()
		{
			return &Position;
		}

		Vec3* Transform::GetGlobalScale()
		{
			return &GlobalScale;
		}

		Vec3* Transform::GetLocalScale()
		{
			return &LocalScale;
		}

		void Transform::Reset()
		{
			GlobalScale.x = 1;
			GlobalScale.y = 1;
			GlobalScale.z = 1;

			LocalScale.x = 1;
			LocalScale.y = 1;
			LocalScale.z = 1;

			LocalRotation.v.x = 0;
			LocalRotation.v.y = 0;
			LocalRotation.v.z = 0;
			LocalRotation.w = 1;

			GlobalRotation.v.x = 0;
			GlobalRotation.v.y = 0;
			GlobalRotation.v.z = 0;
			GlobalRotation.w = 1;

			Position.x = 0;
			Position.y = 0;
			Position.z = 0;
		}
	};
};
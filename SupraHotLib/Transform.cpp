#include "Transform.h"

namespace SupraHot
{
	namespace Math
	{
		Transform::Transform()
		{
			Scale = Vec3(1, 1, 1);
			PreScale = Vec3(1, 1, 1);
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

			Mat4 scalingMatrix;
			scalingMatrix.SetScale(Scale);

			Mat4 preScaleMatrix;
			preScaleMatrix.SetScale(PreScale);

			Transformation = preScaleMatrix * globalRotationMatrix * translationMatrix * localRotationMatrix * scalingMatrix;
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

		void Transform::SetScale(const Vec3 &scale)
		{
			this->Scale = scale;
		}

		void Transform::SetPreScale(const Vec3 &scale)
		{
			this->PreScale = scale;
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

		Vec3* Transform::GetScale()
		{
			return &Scale;
		}

		Vec3* Transform::GetPreScale()
		{
			return &PreScale;
		}

		void Transform::Reset()
		{
			Scale.x = 1;
			Scale.y = 1;
			Scale.z = 1;

			PreScale.x = 1;
			PreScale.y = 1;
			PreScale.z = 1;

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
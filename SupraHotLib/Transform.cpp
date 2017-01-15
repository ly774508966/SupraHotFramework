#include "Transform.h"
#include "Entity.h"

namespace SupraHot
{
	namespace Math
	{
		Transform::Transform()
		{
			GlobalScale = Vec3(1, 1, 1);
			LocalScale = Vec3(1, 1, 1);
			LocalRotation = Quat4(Vec3(0, 0, 0), 0);
			GlobalRotation = Quat4(Vec3(0, 0, 0), 0);

			Parent = nullptr;
		}

		Transform::~Transform()
		{

		}

		Mat4 Transform::GetTransformation()
		{
			if (HasChanged)
			{
				CalculateLocalTransform();
				Transformation = LocalTransformation;
				HasChanged = false;
			}

			if (Parent != nullptr)
			{
				Transformation = Parent->GetTransformation() * LocalTransformation;
			}

			return Transformation;
		}

		void Transform::SetLocalRotation(const Quat4 &rotation)
		{
			LocalRotation = rotation;
			HasChanged = true;
		}

		void Transform::SetGlobalRotation(const Quat4 &rotation)
		{
			GlobalRotation = rotation;
			HasChanged = true;
		}

		void Transform::SetPosition(const Vec3 &position)
		{
			this->Position = position;
			HasChanged = true;
		}

		void Transform::SetGlobalPosition(const Vec3& position)
		{
			this->GlobalPosition = position;
			HasChanged = true;
		}

		void Transform::SetGlobalScale(const Vec3 &scale)
		{
			this->GlobalScale = scale;
			HasChanged = true;
		}

		void Transform::SetLocalScale(const Vec3 &scale)
		{
			this->LocalScale = scale;
			HasChanged = true;
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

		Vec3* Transform::GetGlobalPosition()
		{
			return &GlobalPosition;
		}

		Vec3* Transform::GetGlobalScale()
		{
			return &GlobalScale;
		}

		Vec3* Transform::GetLocalScale()
		{
			return &LocalScale;
		}

		void Transform::SetParent(Transform* transform)
		{
			Parent = transform;
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

			GlobalPosition.x = 0;
			GlobalPosition.y = 0;
			GlobalPosition.z = 0;

			HasChanged = true;
		}

		void Transform::CalculateLocalTransform()
		{
			LocalTransformation.Identity();
			LocalRotation.Normalize();
			GlobalRotation.Normalize();

			Mat4 localRotationMatrix;
			localRotationMatrix = localRotationMatrix.ToRotationMatrix(LocalRotation);

			Mat4 translationMatrix;
			translationMatrix.SetTranslationVector(Position);

			Mat4 globalTranslationMatrix;
			globalTranslationMatrix.SetTranslationVector(GlobalPosition);

			Mat4 globalRotationMatrix;
			globalRotationMatrix = globalRotationMatrix.ToRotationMatrix(GlobalRotation);

			Mat4 globalScaleMatrix;
			globalScaleMatrix.SetScale(GlobalScale);

			Mat4 localScaleMatrix;
			localScaleMatrix.SetScale(LocalScale);

			LocalTransformation = (translationMatrix * localRotationMatrix * localScaleMatrix) * (globalTranslationMatrix * globalRotationMatrix * globalScaleMatrix);
		}
	};
};
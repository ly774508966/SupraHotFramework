#include "Transform.h"
#include "Entity.h"

namespace SupraHot
{
	namespace Math
	{
		Transform::Transform()
		{
			LocalScale = Vec3(1, 1, 1);
			GlobalScale = Vec3(1, 1, 1);
			Rotation = Quat4(Vec3(0, 0, 0), 1);
			LocalPosition = Vec3(0, 0, 0);
			GlobalPosition = Vec3(0, 0, 0);

			Parent = nullptr;
			HasChanged = true;
		}

		Transform::~Transform()
		{

		}

		Mat4 Transform::GetTransformation()
		{			
			UpdateTransform();
			return Transformation;
		}

		void Transform::SetRotation(const Quat4 &rotation)
		{
			this->Rotation = rotation;
			HasChanged = true;
		}

		void Transform::SetLocalPosition(const Vec3 &position)
		{
			this->LocalPosition = position;
			HasChanged = true;
		}

		void Transform::SetLocalScale(const Vec3 &scale)
		{
			this->LocalScale = scale;
			HasChanged = true;
		}

		Vec3 Transform::GetTotalScale()
		{
			if (HasParent())
			{
				return LocalScale * Parent->GetTotalScale();
			}

			return LocalScale;
		}

		void Transform::SetGlobalScale(const Vec3& scale)
		{
			// Get Parent's Scale
			Vec3 parentScale = GetTotalScale();
			parentScale.x /= LocalScale.x;
			parentScale.y /= LocalScale.y;
			parentScale.z /= LocalScale.z;

			Vec3 targetScale(scale.x / parentScale.x, scale.y / parentScale.y, scale.z / parentScale.z);
			SetLocalScale(targetScale);
			
			GlobalScale = scale;			
			HasChanged = true;
		}

		Quat4* Transform::GetRotation()
		{
			return &Rotation;
		}

		Vec3* Transform::GetLocalPosition()
		{
			return &LocalPosition;
		}

		Vec3* Transform::GetLocalScale()
		{
			return &LocalScale;
		}

		Vec3* Transform::GetGlobalScale()
		{
			return &GlobalScale;
		}

		Vec3 Transform::GetGlobalPosition()
		{
			UpdateTransform();
			return GlobalPosition;
		}

		// Hint: This will override the local position, so that the final transformed position will be
		// this position.
		void Transform::SetGlobalPosition(const Vec3& position)
		{
			SetLocalPosition(Vec3(0, 0, 0));
			UpdateTransform();

			if (HasParent())
			{
				// Todo: Could also cache the inverse matrices from the UpdateTransform-Method!
				Mat4 inverseTransformation = Parent->GetTransformation().Inversed();
				SetLocalPosition(inverseTransformation * position);
			}
			else
			{
				SetLocalPosition(position);
			}
		}

		void Transform::SetParent(Transform* transform)
		{
			Parent = transform;
		}

		void Transform::Reset()
		{
			LocalScale.x = 1;
			LocalScale.y = 1;
			LocalScale.z = 1;

			GlobalScale.x = 1;
			GlobalScale.y = 1;
			GlobalScale.z = 1;

			Rotation.v.x = 0;
			Rotation.v.y = 0;
			Rotation.v.z = 0;
			Rotation.w = 1;

			LocalPosition.x = 0;
			LocalPosition.y = 0;
			LocalPosition.z = 0;

			GlobalPosition.x = 0;
			GlobalPosition.y = 0;
			GlobalPosition.z = 0;

			HasChanged = true;
		}

		bool Transform::HasParent()
		{
			return Parent != nullptr;
		}

		void Transform::CalculateLocalTransform()
		{
			LocalTransformation.Identity();
			Rotation.Normalize();

			Mat4 rotationMatrix;
			rotationMatrix = rotationMatrix.ToRotationMatrix(Rotation);

			Mat4 translationMatrix;
			translationMatrix.SetTranslationVector(LocalPosition);

			Mat4 scaleMatrix;
			scaleMatrix.SetScale(LocalScale);

			LocalTransformation = translationMatrix * rotationMatrix * scaleMatrix;
		}

		void Transform::UpdateTransform()
		{
			if (HasChanged)
			{
				CalculateLocalTransform();
				Transformation = LocalTransformation;
				HasChanged = false;
			}

			// Todo: Need a clever way to determine, if the parent has changed or not.
			// Maybe we need to run this through the update-loop itself.
			
			// If parent has changed, update global position. (Recalculate it)
			if (HasParent())
			{
				Transformation = Parent->GetTransformation() * LocalTransformation;
			}

			GlobalScale = GetTotalScale();
			GlobalPosition = Transformation * Vec3(0, 0, 0);
		}

		void Transform::Blend(Transform& t, float alpha)
		{
			SetLocalPosition(this->LocalPosition.Lerp(*t.GetLocalPosition(), alpha));
			SetRotation(this->Rotation.Slerp(*t.GetRotation(), alpha));
			SetLocalScale(this->LocalScale.Lerp(*t.GetLocalScale(), alpha));
			Rotation.Normalize();
		}

		Transform Transform::Blended(Transform& t, float alpha)
		{
			Transform out = *this;
			out.Blend(t, alpha);
			return out;
		}
	};
};
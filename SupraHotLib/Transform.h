#pragma once
#include "Vec3.h"
#include "Quat4.h"
#include "Mat4.h"

namespace SupraHot
{
	namespace Math
	{
		class Transform
		{
		private:
			Quat4 Rotation;
			Vec3 LocalPosition;
			Vec3 LocalScale;

			Mat4 Transformation;
			Mat4 LocalTransformation;

			Transform* Parent;

			// This var only caches the global pos
			Vec3 GlobalPosition;

			// This var only caches the global scale
			Vec3 GlobalScale;

			// Helper function to get only the parent's scale.
			Vec3 GetTotalScale();
		public:
			bool HasChanged = true;

			Transform();
			~Transform();

			void SetRotation(const Quat4& rotation);
			void SetLocalPosition(const Vec3& position);
			void SetLocalScale(const Vec3& scale);
			void SetGlobalScale(const Vec3& scale);

			Quat4* GetRotation();
			Vec3* GetLocalPosition();
			Vec3* GetLocalScale();
			Vec3* GetGlobalScale();

			Vec3 GetGlobalPosition();
			void SetGlobalPosition(const Vec3& position);

			void SetParent(Transform* transform);
			Mat4 GetTransformation();

			void Reset();
			bool HasParent();

			void Blend(Transform& t, float alpha);
			Transform Blended(Transform& t, float alpha);
		private:
			void CalculateLocalTransform();
			void UpdateTransform();
		};
	};
};

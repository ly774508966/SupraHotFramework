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
			Quat4 LocalRotation;
			Quat4 GlobalRotation;

			Vec3 Position;
			Vec3 GlobalScale;
			Vec3 LocalScale;

			Mat4 Transformation;
			Mat4 LocalTransformation;

			Transform* Parent;
		public:
			bool HasChanged = true;

			Transform();
			~Transform();

			void SetLocalRotation(const Quat4& rotation);
			void SetGlobalRotation(const Quat4& rotation);
			void SetPosition(const Vec3& position);
			void SetGlobalScale(const Vec3& scale);
			void SetLocalScale(const Vec3& scale);

			Quat4* GetLocalRotation();
			Quat4* GetGlobalRotation();

			Vec3* GetPosition();
			Vec3* GetGlobalScale();
			Vec3* GetLocalScale();

			void SetParent(Transform* transform);
			Mat4 GetTransformation();

			void Reset();
		private:
			void CalculateLocalTransform();
		};
	};
};

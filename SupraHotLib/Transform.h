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
			Vec3 Scale;
			Vec3 PreScale;

			Mat4 Transformation;
		public:
			Transform();
			~Transform();

			void SetLocalRotation(const Quat4& rotation);
			void SetGlobalRotation(const Quat4& rotation);
			void SetPosition(const Vec3& position);
			void SetScale(const Vec3& scale);
			void SetPreScale(const Vec3& scale);

			Quat4* GetLocalRotation();
			Quat4* GetGlobalRotation();

			Vec3* GetPosition();
			Vec3* GetScale();
			Vec3* GetPreScale();

			Mat4 GetTransformation();

			void Reset();
		};
	};
};

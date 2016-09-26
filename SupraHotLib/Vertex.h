#pragma once
#include "Platform.h"
#include "Vec3.h"
#include "Vec2.h"

namespace SupraHot
{
	namespace Graphics
	{
		using namespace Math;

		class Vertex
		{
		private:
			Vec3 Position;
			Vec3 Normal;
			Vec3 Tangent;
			Vec3 BiNormal;
			Vec2 UvCoord;
		public:
			Vertex();
			~Vertex();

			void SetPosition(const Vec3& position);
			void SetNormal(const Vec3& normal);
			void SetTangent(const Vec3& tangent);
			void SetBiNormal(const Vec3& biNormal);
			void SetUVCoord(const Vec2& uvCoord);

			Vec3 GetPosition();
			Vec3 GetNormal();
			Vec3 GetTangent();
			Vec3 GetBiNormal();
			Vec2 GetUVCoord();
		};
	};
};


#pragma once
#include "Platform.h"
#include "Light.h"

namespace SupraHot
{
	namespace Graphics
	{
		class PointLight
		{
		private:
			Light Light;
			Vec3 Position;
			float Radius;
		public:
			PointLight(Graphics::Light light = Graphics::Light(), Vec3 position = Vec3(), float radius = 1.0);
			~PointLight();

			Graphics::Light& GetLight();
			void SetLight(Graphics::Light light);

			Vec3& GetPosition();
			void SetPosition(Vec3 position);

			float& GetRadius();
			void SetRadius(float radius);
		};
	};
};
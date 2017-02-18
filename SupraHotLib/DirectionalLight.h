#pragma once
#include "Platform.h"
#include "Light.h"

namespace SupraHot
{
	namespace Graphics
	{
		class DirectionalLight
		{
		private:
			Light Light;
			Vec3 Direction;
		public:
			DirectionalLight(Graphics::Light light = Graphics::Light(), Vec3 direction = Vec3(0, -1, 0));
			~DirectionalLight();

			Graphics::Light& GetLight();
			void SetLight(Graphics::Light light);

			Vec3& GetDirection();
			void SetDirection(Vec3 direction);
		};
	};
};
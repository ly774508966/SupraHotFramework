#pragma once
#include "Platform.h"
#include "Vec3.h"

namespace SupraHot
{

	namespace Graphics
	{
		using namespace Math;

		class Light
		{
		private:
			Vec3 Color;
			float Intensity;
		public:
			Light(Vec3 color = Vec3(), float intensity = 1.0f);
			~Light();

			Vec3& GetColor();
			float& GetIntensity();

			void SetColor(Vec3 color);
			void SetIntensity(float intensity);
		};
	};
};
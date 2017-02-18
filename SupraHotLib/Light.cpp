#include "Light.h"

namespace SupraHot
{
	namespace Graphics
	{
		Light::Light(Vec3 color, float intensity)
		{
			SetColor(color);
			SetIntensity(intensity);
		}

		Light::~Light()
		{
		}

		Vec3& Light::GetColor()
		{
			return Color;
		}

		float& Light::GetIntensity()
		{
			return Intensity;
		}

		void Light::SetColor(Vec3 color)
		{
			Color = color;
		}

		void Light::SetIntensity(float intensity)
		{
			Intensity = intensity;
		}
	};
};
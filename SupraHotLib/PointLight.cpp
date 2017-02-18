#include "PointLight.h"

namespace SupraHot
{
	namespace Graphics
	{
		PointLight::PointLight(Graphics::Light light, Vec3 position, float radius)
		{
			SetLight(light);
			SetPosition(position);
			SetRadius(radius);
		}

		PointLight::~PointLight()
		{
		}

		Graphics::Light& PointLight::GetLight()
		{
			return Light;
		}

		void PointLight::SetLight(Graphics::Light light)
		{
			Light = light;
		}

		Vec3& PointLight::GetPosition()
		{
			return Position;
		}

		void PointLight::SetPosition(Vec3 position)
		{
			Position = position;
		}

		float& PointLight::GetRadius()
		{
			return Radius;
		}

		void PointLight::SetRadius(float radius)
		{
			Radius = radius;
		}
	};
};
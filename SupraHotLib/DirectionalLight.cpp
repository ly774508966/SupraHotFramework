#include "DirectionalLight.h"

namespace SupraHot
{
	namespace Graphics
	{
		DirectionalLight::DirectionalLight(Graphics::Light light, Vec3 direction)
		{
			SetLight(light);
			SetDirection(direction);
		}

		DirectionalLight::~DirectionalLight()
		{
		}

		Graphics::Light& DirectionalLight::GetLight()
		{
			return Light;
		}

		void DirectionalLight::SetLight(Graphics::Light light)
		{
			Light = light;
		}

		Vec3& DirectionalLight::GetDirection()
		{
			return Direction;
		}

		void DirectionalLight::SetDirection(Vec3 direction)
		{
			Direction = direction;
		}
	};
};
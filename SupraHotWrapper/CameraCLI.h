#pragma once
#include "Wrapper.h"
#include <Camera.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class CameraCLI : Wrapper<SupraHot::Graphics::Camera>
		{
		public:
			CameraCLI();
			void SetInstance(SupraHot::Graphics::Camera* camera);
			float GetYaw();
			float GetPitch();
			void SetYaw(float yaw);
			void SetPitch(float pitch);

			void MoveRight(float amount);
			void MoveUp(float amount);
			void MoveForward(float amount);
		};
	};
};
#include "CameraCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		CameraCLI::CameraCLI()
		{
		}

		float CameraCLI::GetYaw()
		{
			return Instance->Yaw;
		}

		float CameraCLI::GetPitch()
		{
			return Instance->Pitch;
		}

		void CameraCLI::SetYaw(float yaw)
		{
			Instance->Yaw = yaw;
		}

		void CameraCLI::SetPitch(float pitch)
		{
			Instance->Pitch = pitch;
		}

		void CameraCLI::MoveRight(float amount)
		{
			Instance->Position += Instance->GetQuaternion().GetRight().Normalized() * amount * Instance->MoveSpeed;
		}

		void CameraCLI::MoveUp(float amount)
		{
			Instance->Position += Instance->GetQuaternion().GetUp().Normalized() * amount * Instance->MoveSpeed;
		}

		void CameraCLI::MoveForward(float amount)
		{
			Instance->Position += Instance->GetQuaternion().GetForward().Normalized() * amount * Instance->MoveSpeed;
		}

		void CameraCLI::SetInstance(SupraHot::Graphics::Camera* camera)
		{
			if (Instance != nullptr)
			{
				delete Instance;
			}
			Instance = camera;
		}
	};
};

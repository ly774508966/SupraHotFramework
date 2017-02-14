#include "SetCameraMatricesCmd.h"
#include "Camera.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		SetCameraMatricesCmd::SetCameraMatricesCmd(Graphics::Camera* camera)
		{
			this->Camera = camera;
		}

		SetCameraMatricesCmd::~SetCameraMatricesCmd()
		{

		}

		void SetCameraMatricesCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveCamera = Camera;
		}
	};
};
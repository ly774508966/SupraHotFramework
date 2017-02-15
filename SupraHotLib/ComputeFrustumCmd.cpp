#include "ComputeFrustumCmd.h"
#include "RenderCommandQueue.h"
#include "Frustum.h"
#include "Camera.h"

namespace SupraHot
{
	namespace Graphics
	{
		ComputeFrustumCmd::ComputeFrustumCmd(Graphics::Frustum* frustum)
		{
			this->Frustum = frustum;
		}

		ComputeFrustumCmd::~ComputeFrustumCmd()
		{

		}

		void ComputeFrustumCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveFrustum = Frustum;
			renderCommandQueueState->ActiveFrustum->Compute(*renderCommandQueueState->ActiveCamera->GetViewProjectionMatrix());
		}
	};
};

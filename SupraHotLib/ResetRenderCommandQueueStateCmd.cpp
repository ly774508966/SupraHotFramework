#include "ResetRenderCommandQueueStateCmd.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		ResetRenderCommandQueueStateCmd::ResetRenderCommandQueueStateCmd()
		{
		}

		ResetRenderCommandQueueStateCmd::~ResetRenderCommandQueueStateCmd()
		{
		}

		void ResetRenderCommandQueueStateCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveShader = nullptr;
			renderCommandQueueState->ActiveMeshComponent = nullptr;
			renderCommandQueueState->ActiveMaterial = nullptr;
			renderCommandQueueState->ActiveCamera = nullptr;
			renderCommandQueueState->ActiveFrustum = nullptr;
			renderCommandQueueState->ActiveBRDF = Graphics::Shader::BRDFType::None;
		}
	};
};
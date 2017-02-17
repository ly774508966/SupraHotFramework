#include "UnbindShaderCmd.h"
#include "Shader.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		UnbindShaderCmd::UnbindShaderCmd()
		{
		}

		UnbindShaderCmd::~UnbindShaderCmd()
		{
		}

		void UnbindShaderCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveShader->Detach();
			renderCommandQueueState->ActiveShader = nullptr;			
		}
	};
};

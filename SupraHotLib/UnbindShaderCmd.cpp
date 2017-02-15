#include "UnbindShaderCmd.h"
#include "Shader.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		UnbindShaderCmd::UnbindShaderCmd(Graphics::Shader* shader)
		{
			this->Shader = shader;
		}

		UnbindShaderCmd::~UnbindShaderCmd()
		{
		}

		void UnbindShaderCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveShader = nullptr;
			Shader->Detach();
		}
	};
};

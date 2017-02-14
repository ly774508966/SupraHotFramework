#include "BindShaderCmd.h"
#include "Shader.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		BindShaderCmd::BindShaderCmd(Graphics::Shader* shader)
		{
			this->Shader = shader;
		}

		BindShaderCmd::~BindShaderCmd()
		{
		}

		void BindShaderCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveShader = Shader;
			Shader->Attach();
		}
	};
};

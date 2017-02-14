#include "UnbindShaderCmd.h"
#include "Shader.h"

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
			Shader->Detach();
		}
	};
};

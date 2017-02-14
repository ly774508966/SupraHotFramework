#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Shader;

		class BindShaderCmd : public RenderCommand
		{
		private:
			Shader* Shader;
		public:
			BindShaderCmd(Graphics::Shader* shader);
			~BindShaderCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};
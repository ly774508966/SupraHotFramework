#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Shader;

		class UnbindShaderCmd : public RenderCommand
		{
		private:
			Shader* Shader;
		public:
			UnbindShaderCmd(Graphics::Shader* shader);
			~UnbindShaderCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};
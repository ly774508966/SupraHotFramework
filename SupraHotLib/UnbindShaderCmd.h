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
		public:
			UnbindShaderCmd();
			~UnbindShaderCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};
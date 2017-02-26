#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class GBuffer;

		class UnbindGBufferCmd : public RenderCommand
		{
		private:
			Graphics::GBuffer* GBuffer;
		public:
			UnbindGBufferCmd(Graphics::GBuffer* gBuffer);
			~UnbindGBufferCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};
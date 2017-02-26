#pragma once

#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class GBuffer;

		class BindGBufferCmd : public RenderCommand
		{
		private:
			Graphics::GBuffer* GBuffer;
		public:
			BindGBufferCmd(Graphics::GBuffer* gBuffer);
			~BindGBufferCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};


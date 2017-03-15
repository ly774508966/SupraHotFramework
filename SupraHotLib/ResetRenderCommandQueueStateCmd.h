#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class ResetRenderCommandQueueStateCmd : public RenderCommand
		{	
		public:
			ResetRenderCommandQueueStateCmd();
			~ResetRenderCommandQueueStateCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};
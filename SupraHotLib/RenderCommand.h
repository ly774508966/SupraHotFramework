#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		struct RenderCommandQueueState;
		class RenderCommand
		{
		public:
			RenderCommand();
			virtual ~RenderCommand();
			virtual void Execute(RenderCommandQueueState* renderCommandQueueState) = 0;
		};
	};
};
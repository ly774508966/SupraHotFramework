#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Frustum;

		class ComputeFrustumCmd : public RenderCommand
		{
		private:
			Graphics::Frustum* Frustum;
		public:
			ComputeFrustumCmd(Graphics::Frustum* frustum);
			~ComputeFrustumCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};

	};
};
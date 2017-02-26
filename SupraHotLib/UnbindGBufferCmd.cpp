#include "UnbindGBufferCmd.h"
#include "GBuffer.h"

namespace SupraHot
{
	namespace Graphics
	{
		UnbindGBufferCmd::UnbindGBufferCmd(Graphics::GBuffer* gBuffer)
		{
			this->GBuffer = gBuffer;
		}


		UnbindGBufferCmd::~UnbindGBufferCmd()
		{
		}

		void UnbindGBufferCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			this->GBuffer->Detach();
		}
	};
};

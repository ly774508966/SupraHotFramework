#include "BindGBufferCmd.h"
#include "GBuffer.h"

namespace SupraHot
{
	namespace Graphics
	{
		BindGBufferCmd::BindGBufferCmd(Graphics::GBuffer* gBuffer)
		{
			this->GBuffer = gBuffer;
		}

		BindGBufferCmd::~BindGBufferCmd()
		{

		}

		void BindGBufferCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			this->GBuffer->Attach();
		}
	};
};
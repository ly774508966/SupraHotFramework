#include "BindBRDFRenderTargetsCmd.h"

namespace SupraHot
{
	namespace Graphics
	{
		BindBRDFRenderTargetsCmd::BindBRDFRenderTargetsCmd(Graphics::Shader::BRDFType brdfType)
		{
			BRDFType = brdfType;
		}


		BindBRDFRenderTargetsCmd::~BindBRDFRenderTargetsCmd()
		{
		}

		void BindBRDFRenderTargetsCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			// Bind that puppy.
		}
	};
};
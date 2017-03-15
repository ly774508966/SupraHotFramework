#include "BindBRDFRenderTargetsCmd.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		BindBRDFRenderTargetsCmd::BindBRDFRenderTargetsCmd(Graphics::Shader::BRDFType brdfType)
		{
			BRDFType = brdfType;
			
			SHF_PRINTF("Bind BRDF : %d \n", BRDFType);

		}


		BindBRDFRenderTargetsCmd::~BindBRDFRenderTargetsCmd()
		{
		}

		void BindBRDFRenderTargetsCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveBRDF = BRDFType;

			// Bind that puppy.
		}
	};
};
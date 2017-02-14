#include "RenderMeshCmd.h"
#include "MeshData.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		RenderMeshCmd::RenderMeshCmd(Graphics::MeshData* rawMeshData)
		{
			this->MeshData = rawMeshData;
		}

		RenderMeshCmd::~RenderMeshCmd()
		{
		}

		void RenderMeshCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveMeshData = MeshData;
		}
	};
};

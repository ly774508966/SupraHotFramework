#include "BindMaterialCmd.h"
#include "Material.h"
#include "RenderCommandQueue.h"

namespace SupraHot
{
	namespace Graphics
	{
		BindMaterialCmd::BindMaterialCmd(Graphics::Material* material)
		{
			this->Material = material;
		}

		BindMaterialCmd::~BindMaterialCmd()
		{
		}

		void BindMaterialCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveMaterial = Material;
			Material->GetMaterialInputs()->Apply(Material->GetShader());
		}
	};
};
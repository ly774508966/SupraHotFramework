#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	class MeshComponent;

	namespace Graphics
	{
		class MeshData;

		class RenderMeshCmd : public RenderCommand
		{
		private:
			MeshComponent* MeshComponent;
		public:
			RenderMeshCmd(SupraHot::MeshComponent* meshComponent);
			~RenderMeshCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};


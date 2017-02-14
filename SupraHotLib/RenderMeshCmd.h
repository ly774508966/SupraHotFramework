#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class MeshData;

		class RenderMeshCmd : public RenderCommand
		{
		private:
			Graphics::MeshData* MeshData;
		public:
			RenderMeshCmd(Graphics::MeshData* rawMeshData);
			~RenderMeshCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};


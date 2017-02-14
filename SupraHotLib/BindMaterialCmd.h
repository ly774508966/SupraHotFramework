#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Material;

		class BindMaterialCmd : public RenderCommand
		{
		private:
			Graphics::Material* Material;
		public:
			BindMaterialCmd(Graphics::Material* material);
			~BindMaterialCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};

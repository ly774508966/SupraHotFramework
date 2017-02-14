#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Camera;

		class SetCameraMatricesCmd : public RenderCommand
		{
		private:
			Graphics::Camera* Camera;
		public:
			SetCameraMatricesCmd(Graphics::Camera* camera);
			~SetCameraMatricesCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};


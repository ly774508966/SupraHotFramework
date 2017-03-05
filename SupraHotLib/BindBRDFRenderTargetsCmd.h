#pragma once
#include "Platform.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace SupraHot
{
	namespace Graphics
	{	
		class BindBRDFRenderTargetsCmd : public RenderCommand
		{	
		private:
			Graphics::Shader::BRDFType BRDFType = Graphics::Shader::BRDFType::None;
		public:
			BindBRDFRenderTargetsCmd(Graphics::Shader::BRDFType brdfType);
			~BindBRDFRenderTargetsCmd();
			void Execute(RenderCommandQueueState* renderCommandQueueState) override;
		};
	};
};
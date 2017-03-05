#pragma once
#include "Platform.h"
#include "RenderCommand.h"

namespace SupraHot
{
	namespace Graphics
	{
		class BindBRDFRenderTargetsCmd : public RenderCommand
		{
		public:
			BindBRDFRenderTargetsCmd();
			~BindBRDFRenderTargetsCmd();
		};
	};
};
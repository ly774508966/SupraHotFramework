#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class RenderCommand
		{
		public:
			RenderCommand();
			virtual ~RenderCommand();
			virtual void Execute() = 0;
		};
	};
};
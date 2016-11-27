#pragma once
#include "Wrapper.h"
#include <MeshComponent.h>
#if 0
namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshComponent : Wrapper<SupraHot::MeshComponent>
		{
		public:
			MeshComponent()  override
			{
				Instance = new SupraHot::MeshComponent(nullptr, nullptr);
			};
		};
	};
};
#endif
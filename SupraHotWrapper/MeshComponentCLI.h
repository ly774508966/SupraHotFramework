#pragma once
#include "Wrapper.h"
#include <MeshComponent.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshComponent : Wrapper<SupraHot::MeshComponent>
		{
		public:
			MeshComponent();
			void SetMeshData();
			void SetMaterial();
		};
	};
};

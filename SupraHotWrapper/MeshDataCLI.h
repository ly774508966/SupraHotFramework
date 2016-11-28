#pragma once
#include "Wrapper.h"
#include <MeshData.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshDataCLI : public Wrapper<Graphics::MeshData>
		{
		public:
			MeshDataCLI();
		};
	};
};
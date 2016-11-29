#pragma once
#include "Wrapper.h"
#include <MeshData.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshData : public Wrapper<Graphics::MeshData>
		{
		public:
			MeshData();
		};
	};
};
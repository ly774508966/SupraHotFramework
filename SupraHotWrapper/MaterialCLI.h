#pragma once
#include "Wrapper.h"
#include <Material.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Material : public Wrapper<Graphics::Material>
		{
		public:
			Material();
			Material(Graphics::Material* instance);
			System::String^ GetName();
		};
	};
};
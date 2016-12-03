#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCommonInterface.h"
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

			System::Collections::Generic::List<MaterialPropertyCommonInterface^>^ GetMaterialProperties();
		};
	};
};
#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCommonInterface.h"
#include <ShaderMaterial.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Material : public Wrapper<Graphics::ShaderMaterial>
		{
		public:
			Material();
			Material(Graphics::ShaderMaterial* instance);
			System::String^ GetName();

			System::Collections::Generic::List<MaterialPropertyCommonInterface^>^ GetMaterialProperties();
		};
	};
};
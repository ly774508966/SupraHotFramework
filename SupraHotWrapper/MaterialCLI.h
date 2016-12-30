#pragma once
#include "Wrapper.h"
#include "BooleanMaterialPropertyCLI.h"
#include "FloatMaterialPropertyCLI.h"
#include "Vec2MaterialPropertyCLI.h"
#include "Vec3MaterialPropertyCLI.h"
#include "Vec4MaterialPropertyCLI.h"
#include "Texture2DMaterialPropertyCLI.h"
#include "TextureCubeMaterialPropertyCLI.h"
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
			MaterialPropertyCommonInterface^ AddMaterialProperty(System::String^ name, System::String^ type);
			void RemoveMaterialProperty(MaterialPropertyCommonInterface^ materialProperty);
			void RemoveMaterialProperty(System::String^ materialName);
			void RemoveAllMaterialProperties();
		};
	};
};
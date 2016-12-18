#include "MaterialCLI.h"
#include "BooleanMaterialPropertyCLI.h"
#include "FloatMaterialPropertyCLI.h"
#include "Vec2MaterialPropertyCLI.h"
#include "Vec3MaterialPropertyCLI.h"
#include "Vec4MaterialPropertyCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		Material::Material()
		{
			
		}

		Material::Material(Graphics::ShaderMaterial* instance)
		{
			Instance = instance;
		}

		System::String^ Material::GetName()
		{
			std::string& name = Instance->Name;
			return msclr::interop::marshal_as<System::String^>(name);
		}

		System::Collections::Generic::List<MaterialPropertyCommonInterface^>^ Material::GetMaterialProperties()
		{
			System::Collections::Generic::List<MaterialPropertyCommonInterface^>^ mpcis = gcnew System::Collections::Generic::List<MaterialPropertyCommonInterface^>();

			std::vector<SupraHot::Graphics::MaterialProperty*>* materialProperties = Instance->GetMaterialProperties();

			for (size_t i = 0, l = materialProperties->size(); i < l; ++i)
			{
				SupraHot::Graphics::MaterialProperty* materialProperty = materialProperties->at(i);

				SHF_PRINTF("Material type => %s \n", materialProperty->GetType().c_str());

				if (materialProperty->GetType() == "Boolean")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::BooleanMaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::BooleanMaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "Float")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::FloatMaterialPropertyCLI
						(
						static_cast<SupraHot::Graphics::FloatMaterialProperty*>(materialProperty)
						)
						);
				}
				else if (materialProperty->GetType() == "Vec2")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Vec2MaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Vec2MaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "Vec3")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Vec3MaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Vec3MaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "Vec4")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Vec4MaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Vec4MaterialProperty*>(materialProperty)
						)
					);
				}



				// #4 Create MaterialPropertiesLIB for Texture2D & TextureCube
				// #5 Craete MaterialPRopertiesCLI for "" & ""
				// #6 Create CLI structs for Texture2d & texturecube
				// #7 wire them up here, and in the lib, + C# FrontEnd.

			}


			return mpcis;
		}
	};
};
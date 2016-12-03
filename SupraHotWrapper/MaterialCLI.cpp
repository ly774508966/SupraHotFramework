#include "MaterialCLI.h"
#include "BooleanMaterialPropertyCLI.h"
#include "Vec3MaterialPropertyCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		Material::Material()
		{
			
		}

		Material::Material(Graphics::Material* instance)
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

			 

			for (SupraHot::Graphics::MaterialProperty* materialProperty : Instance->MaterialProperties)
			{
				
				if (materialProperty->GetType() == "Boolean")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::BooleanMaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::BooleanMaterialProperty*>(materialProperty)
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


			}


			return mpcis;
		}
	};
};
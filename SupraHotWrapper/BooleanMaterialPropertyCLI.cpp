#include "BooleanMaterialPropertyCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		BooleanMaterialPropertyCLI::BooleanMaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::BooleanMaterialProperty(std_name);
		}

		System::String^ BooleanMaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ BooleanMaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};

		BooleanMaterialPropertyCLI::BooleanMaterialPropertyCLI(SupraHot::Graphics::BooleanMaterialProperty* bmp)
		{
			Instance = bmp;
		}
	};
};

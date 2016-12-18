#include "FloatMaterialPropertyCLI.h"

#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		FloatMaterialPropertyCLI::FloatMaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::FloatMaterialProperty(std_name);
		}

		System::String^ FloatMaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ FloatMaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};

		FloatMaterialPropertyCLI::FloatMaterialPropertyCLI(SupraHot::Graphics::FloatMaterialProperty*  v3mp)
		{
			Instance = v3mp;
			IsCopy = true;
		};
	};
};

#include "Vec4MaterialPropertyCLI.h"

#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		Vec4MaterialPropertyCLI::Vec4MaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::Vec4MaterialProperty(std_name);
		}

		System::String^ Vec4MaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ Vec4MaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};

		Vec4MaterialPropertyCLI::Vec4MaterialPropertyCLI(SupraHot::Graphics::Vec4MaterialProperty*  v3mp)
		{
			Instance = v3mp;
			IsCopy = true;
		};
	};
};

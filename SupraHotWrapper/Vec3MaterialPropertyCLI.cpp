#include "Vec3MaterialPropertyCLI.h"

#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		Vec3MaterialPropertyCLI::Vec3MaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::Vec3MaterialProperty(std_name);
		}

		System::String^ Vec3MaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ Vec3MaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};
	};
};

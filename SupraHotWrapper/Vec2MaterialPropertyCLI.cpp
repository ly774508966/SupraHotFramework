#include "Vec2MaterialPropertyCLI.h"

#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		Vec2MaterialPropertyCLI::Vec2MaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::Vec2MaterialProperty(std_name);
		}

		System::String^ Vec2MaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ Vec2MaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};

		Vec2MaterialPropertyCLI::Vec2MaterialPropertyCLI(SupraHot::Graphics::Vec2MaterialProperty*  v3mp)
		{
			Instance = v3mp;
			IsCopy = true;
		};
	};
};

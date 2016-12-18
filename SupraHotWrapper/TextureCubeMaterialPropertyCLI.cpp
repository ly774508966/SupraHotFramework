#include "TextureCubeMaterialPropertyCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		TextureCubeMaterialPropertyCLI::TextureCubeMaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::TextureCubeMaterialProperty(std_name);
		}

		System::String^ TextureCubeMaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ TextureCubeMaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};

		TextureCubeMaterialPropertyCLI::TextureCubeMaterialPropertyCLI(SupraHot::Graphics::TextureCubeMaterialProperty*  tx2dmp)
		{
			Instance = tx2dmp;
			IsCopy = true;
		};
	};
};

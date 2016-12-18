#include "Texture2DMaterialPropertyCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		Texture2DMaterialPropertyCLI::Texture2DMaterialPropertyCLI(System::String^ name)
		{
			std::string std_name = msclr::interop::marshal_as<std::string>(name);
			Instance = new SupraHot::Graphics::Texture2DMaterialProperty(std_name);
		}

		System::String^ Texture2DMaterialPropertyCLI::GetType()
		{
			const std::string& type = Instance->GetType();
			return msclr::interop::marshal_as<System::String^>(type);
		}

		System::String^ Texture2DMaterialPropertyCLI::GetName()
		{
			const std::string& name = Instance->GetName();
			return msclr::interop::marshal_as<System::String^>(name);
		};

		Texture2DMaterialPropertyCLI::Texture2DMaterialPropertyCLI(SupraHot::Graphics::Texture2DMaterialProperty*  tx2dmp)
		{
			Instance = tx2dmp;
			IsCopy = true;
		};
	};
};

#pragma once
#include "EntityCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		EntityCLI::EntityCLI()
		{
		}

		void EntityCLI::AddComponent(SupraHot::CLI::ComponentCLI^ component)
		{
			printf("Adding component = %p \n", component);
		}

		generic <class T>
			where T: ComponentCLI, gcnew()
		T EntityCLI::GetComponent()
		{

			System::Type^ type = T::typeid;
			std::string typeName = msclr::interop::marshal_as<std::string>(type->Name);
			std::string getCompString = typeName;

			if (typeName.find("CLI") != std::string::npos)
			{
				getCompString = getCompString.substr(0, typeName.find("CLI"));
			}

			SupraHot::Component* component = Instance->GetComponent(getCompString);
			if (component != nullptr)
			{
				//SupraHot::CLI::ComponentCLI^ componentCLI = gcnew SupraHot::CLI::ComponentCLI(component);
				T converted = gcnew T();
				converted->ReplaceInstance(component);
				converted->IsCopy = true;
				return converted;
			}

			return T();
		}

		System::String^ EntityCLI::GetName()
		{
			return msclr::interop::marshal_as<System::String^>(Instance->GetName());
		};
	}
};
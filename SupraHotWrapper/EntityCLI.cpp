#pragma once
#include "EntityCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		EntityCLI::EntityCLI()
		{
			//Children = gcnew System::Collections::Generic::List<EntityCLI^>();
		}

		EntityCLI::~EntityCLI()
		{
			SHF_PRINTF("Deleting Entity [~EntityCLI]\nWe destroy it's components here \n");
			GetHandle()->RemoveAndDeleteAllComponents();
		}

		void EntityCLI::AddComponent(SupraHot::CLI::ComponentCLI^ component)
		{
			SHF_PRINTF("EntityCLI::AddComponent NOT IMPLEMENTED \n");
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

			// First check local list
			/*for (int i = 0, l = Components.Count; i < l; ++i)
			{
				SupraHot::CLI::ComponentCLI^ component = Components[i];

				if (getCompString == (component->GetHandle()->Identifier).c_str())
				{
					return static_cast<T>(component);
				}
			}*/


			SupraHot::Component* component = Instance->GetComponent(getCompString);
			if (component != nullptr)
			{
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

		void EntityCLI::SetName(System::String^ name)
		{
			Instance->SetName(msclr::interop::marshal_as<std::string>(name));
		}

		void EntityCLI::AddChild(EntityCLI^ child)
		{
			Instance->AddChild(child->Instance);
		}

		void EntityCLI::RemoveChild(EntityCLI^ child)
		{
			Instance->RemoveChild(child->Instance);
		}

		System::Collections::Generic::List<EntityCLI^>^ EntityCLI::GetChildren()
		{
			std::vector<Entity*>* children = Instance->GetChildren();
			System::Collections::Generic::List<SupraHot::CLI::EntityCLI^>^ childrenCLI = gcnew System::Collections::Generic::List<SupraHot::CLI::EntityCLI^>();

			for (size_t i = 0, l = children->size(); i < l; ++i)
			{
				SupraHot::CLI::EntityCLI^ entityCLI = gcnew SupraHot::CLI::EntityCLI();
				entityCLI->ReplaceInstance(children->at(i));
				entityCLI->IsCopy = true;
				childrenCLI->Add(entityCLI);
			}

			return childrenCLI;
		}

		void EntityCLI::DetachFromParent()
		{
			if (Instance->HasParent())
			{
				SupraHot::Entity* currentParent = Instance->GetParent();
				currentParent->RemoveChild(Instance);
			}
			else
			{
				SHF_PRINTF("Called EntityCLI::DetachFromParent(), but this Entity does not have a parent! \n");
			}
		}
	}
};
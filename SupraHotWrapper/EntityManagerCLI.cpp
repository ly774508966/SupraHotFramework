#include "EntityManagerCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		EntityManagerCLI::EntityManagerCLI()
		{
			Instance = SupraHot::EntityManager::GetInstance();
		}



		EntityManagerCLI^ EntityManagerCLI::GetInstance()
		{
			if (SingletonInstance == nullptr)
			{
				SingletonInstance = gcnew EntityManagerCLI();
			}

			return SingletonInstance;
		}

		void EntityManagerCLI::DestroyAndDelete()
		{
			Instance->DestroyAndDelete();
		}

		void EntityManagerCLI::AddEntity(EntityCLI^ entityCLI)
		{
			Instance->AddEntity(entityCLI->GetHandle());
		}
	}
}

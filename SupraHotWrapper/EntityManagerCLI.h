#pragma once
#include "Platform.h"
#include "EntityCLI.h"
#include <EntityManager.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class EntityManagerCLI
		{
		private:
			static EntityManagerCLI^ SingletonInstance;

			EntityManagerCLI();
			SupraHot::EntityManager* Instance;
		public:
			static EntityManagerCLI^ GetInstance();
			
			void AddEntity(EntityCLI^ entityCLI);
			void DestroyAndDelete();

			~EntityManagerCLI()
			{
			}

			!EntityManagerCLI()
			{
			}
		};
	};
};
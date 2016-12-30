#include "EntityManager.h"
#include <algorithm>

namespace SupraHot
{
	EntityManager::EntityManager()
	{
	}


	EntityManager::~EntityManager()
	{
	}

	EntityManager* EntityManager::GetInstance()
	{
		static EntityManager* instance(new EntityManager);
		return instance;
	}

	void EntityManager::Destroy()
	{
#if DEVELOPMENT == 1
		SHF_PRINTF("EntityManager::Destroy() \n");
#endif
		for (Entity* entity : Entities)
		{
			entity->Destroy();
		}

		Entities.clear();
	}

	void EntityManager::DestroyAndDelete()
	{
#if DEVELOPMENT == 1
		SHF_PRINTF("EntityManager::DestroyAndDelete() \n");
#endif
		for (Entity* entity : Entities)
		{
			entity->Destroy();
			delete entity;
		}

		Entities.clear();
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		Entities.push_back(entity);
	}

	void EntityManager::RemoveEntity(Entity* entity)
	{
		Entities.erase(std::remove(Entities.begin(), Entities.end(), entity), Entities.end());
	}

	std::vector<Entity*>* EntityManager::GetEntities()
	{
		return &Entities;
	}
}; 
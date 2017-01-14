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
		if (std::find(Entities.begin(), Entities.end(), entity) == Entities.end())
		{
			Entities.push_back(entity);
		}
	}

	void EntityManager::RemoveEntity(Entity* entity)
	{
		if (std::find(Entities.begin(), Entities.end(), entity) != Entities.end())
		{
			Entities.erase(std::remove(Entities.begin(), Entities.end(), entity), Entities.end());
		}
	}

	std::vector<Entity*>* EntityManager::GetEntities()
	{
		return &Entities;
	}

	void EntityManager::Update(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
		{
			Entities.at(i)->Update(deltaTime);
		}
	}

	void EntityManager::LateUpdate(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
		{
			Entities.at(i)->LateUpdate(deltaTime);
		}
	}

	void EntityManager::FixedUpdate(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
		{
			Entities.at(i)->FixedUpdate(deltaTime);
		}
	}

	/*void EntityManager::PostRender()
	{
		for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
		{
			Entities.at(i)->GetTransform().GetTransformation();
			Entities.at(i)->GetTransform().HasChanged = false;
		}
	}*/
}; 
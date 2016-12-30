#pragma once
#include "Platform.h"
#include "Entity.h"

namespace SupraHot
{
	class EntityManager
	{
	private:
		EntityManager();
		std::vector<Entity*> Entities;
	public:
		~EntityManager();

		static EntityManager* GetInstance();

		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);
		std::vector<Entity*>* GetEntities();
		void Destroy();
		void DestroyAndDelete();
	};

};
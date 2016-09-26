#include "Entity.h"
#include <vector>
#include <algorithm>

namespace SupraHot
{
	Entity::Entity()
	{
	}

	Entity::~Entity()
	{
	}

	Transform& Entity::GetTransform()
	{
		return EntityTransform;
	}

	void Entity::SetTransform(const Transform& transform)
	{
		this->EntityTransform = transform;
	}

	Component* Entity::GetComponent(std::string componentName)
	{
		for (size_t i = 0, l = Components.size(); i < l; ++i)
		{
			if (Components.at(i)->Identifier == componentName)
			{
				return Components.at(i);
			}
		}

		return nullptr;
	}

	void Entity::RemoveComponent(Component* component)
	{
		Components.erase(std::remove(Components.begin(), Components.end(), component), Components.end());
	}

	Component* Entity::AddComponent(Component* component)
	{
		this->Components.push_back(component);
		component->Register(this);
		return component;
	}

	void Entity::Update(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(this->Components.size()); i < l; ++i)
		{
			this->Components[i]->Update(deltaTime);
		}
	} 

	void Entity::LateUpdate(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(this->Components.size()); i < l; ++i)
		{
			this->Components[i]->LateUpdate(deltaTime);
		}
	}

	void Entity::FixedUpdate(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(this->Components.size()); i < l; ++i)
		{
			this->Components[i]->FixedUpdate(deltaTime);
		}
	}

	void Entity::Destroy()
	{
		for (uint32 i = 0, l = static_cast<uint32>(this->Components.size()); i < l; ++i)
		{
			this->Components[i]->Destroy();
		}
	}
};
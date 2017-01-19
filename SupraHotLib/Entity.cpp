#include "Entity.h"
#include "MeshDataRenderer.h"
#include "MeshComponent.h"
#include <vector>
#include <algorithm>

namespace SupraHot
{
	Entity::Entity()
	{
		
	}

	void Entity::SetName(std::string name)
	{
		Name = name;
	}

	std::string Entity::GetName()
	{
		return Name;
	}

	Entity::~Entity()
	{

	}

	Transform& Entity::GetTransform()
	{
		return EntityTransform;
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

	std::vector<Component*>* Entity::GetComponents()
	{
		return &Components;
	}

	void Entity::RemoveComponent(Component* component)
	{
		if (std::find(Components.begin(), Components.end(), component) != Components.end())
		{
			Components.erase(std::remove(Components.begin(), Components.end(), component), Components.end());
			component->Remove();
			delete component;
		}
	}

	void Entity::RemoveAndDeleteAllComponents()
	{
		for (uint32 i = 0, l = static_cast<uint32>(Components.size()); i < l; ++i)
		{
			Components.at(i)->Remove();
			delete Components.at(i);
		} Components.clear();

		for (uint32 i = 0, l = static_cast<uint32>(Children.size()); i < l; ++i)
		{
			Children[i]->RemoveAndDeleteAllComponents();
		}
	}

	Component* Entity::AddComponent(Component* component)
	{
		if (std::find(Components.begin(), Components.end(), component) == Components.end())
		{
			Components.push_back(component);
			component->Register(this);			
		}

		return component;
	}

	void Entity::AddChild(Entity* child)
	{
		if (std::find(Children.begin(), Children.end(), child) == Children.end())
		{
			Children.push_back(child);
			child->GetTransform().SetParent(&GetTransform());
			child->Parent = this;
		}
	}

	void Entity::RemoveChild(Entity* child)
	{
		if (std::find(Children.begin(), Children.end(), child) != Children.end())
		{
			Children.erase(std::remove(Children.begin(), Children.end(), child), Children.end());
			child->GetTransform().SetParent(nullptr);
			child->Parent = nullptr;
		}
	}

	std::vector<Entity*>* Entity::GetChildren()
	{
		return &Children;
	}

	Entity* Entity::GetParent()
	{
		return Parent;
	}

	bool Entity::HasParent()
	{
		return Parent != nullptr;
	}

	void Entity::Update(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(Components.size()); i < l; ++i)
		{
			Components[i]->Update(deltaTime);
		}
	} 

	void Entity::LateUpdate(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(Components.size()); i < l; ++i)
		{
			Components[i]->LateUpdate(deltaTime);
		}

		for (uint32 i = 0, l = static_cast<uint32>(Children.size()); i < l; ++i)
		{
			Children[i]->LateUpdate(deltaTime);
		}
	}

	void Entity::FixedUpdate(float deltaTime)
	{
		for (uint32 i = 0, l = static_cast<uint32>(Components.size()); i < l; ++i)
		{
			Components[i]->FixedUpdate(deltaTime);
		}

		for (uint32 i = 0, l = static_cast<uint32>(Children.size()); i < l; ++i)
		{
			Children[i]->FixedUpdate(deltaTime);
		}
	}

	void Entity::Destroy()
	{
		for (uint32 i = 0, l = static_cast<uint32>(Components.size()); i < l; ++i)
		{
			Components[i]->Destroy();
			delete Components[i];
		} Components.clear();

		for (uint32 i = 0, l = static_cast<uint32>(Children.size()); i < l; ++i)
		{
			Children[i]->Destroy();
		}
	}
};
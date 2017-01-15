#pragma once
#include "Platform.h"
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		class Camera;
	}

	class MeshComponent;
	using namespace Math;

	class Entity
	{
	private:
		Transform EntityTransform;
		std::vector<Component*> Components;
		std::string Name;

		Entity* Parent = nullptr;
		std::vector<Entity*> Children;

	public:
		Entity();
		~Entity();

		void SetName(std::string name);
		std::string GetName();

		Transform& GetTransform();

		Component* AddComponent(Component* component);
		Component* GetComponent(std::string componentName);
		std::vector<Component*>* GetComponents();
		void RemoveComponent(Component* component);
		void RemoveAndDeleteAllComponents();

		void AddChild(Entity* child);
		void RemoveChild(Entity* child);
		std::vector<Entity*>* GetChildren();

		void Update(float deltaTime);
		void LateUpdate(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Destroy();
	};
};

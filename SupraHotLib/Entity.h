#pragma once
#include "Platform.h"
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace SupraHot
{
	using namespace Math;
	class Entity
	{
	private:
		Transform EntityTransform;
		std::vector<Component*> Components;

	public:
		Entity();
		~Entity();

		Transform& GetTransform();
		void SetTransform(const Transform& transform);

		Component* AddComponent(Component* component);
		Component* GetComponent(std::string componentName);
		void RemoveComponent(Component* component);

		void Update(float deltaTime);
		void LateUpdate(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Destroy();
	};
};

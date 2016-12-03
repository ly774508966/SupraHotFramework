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

	public:
		Entity();
		~Entity();

		void SetName(std::string name);
		std::string GetName();

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

#pragma once
#include "Platform.h"

namespace SupraHot
{
	class Entity;
	class Component
	{
	protected:
		Entity* Parent;
	public:
		std::string Identifier = "Component";

		Component();
		virtual ~Component();

		void Register(Entity* parent)
		{
			this->Parent = parent;
			Registered();
		};

		void Remove()
		{
			this->Parent = nullptr;
			Removed();
		}
		
		virtual void Registered() = 0;
		virtual void Removed() = 0;
		virtual void Destroy() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void LateUpdate(float deltaTime) = 0;
		virtual void FixedUpdate(float deltaTime) = 0;

		Entity* GetParent()
		{
			return Parent;
		}

		void SetParent(Entity* parent)
		{
			this->Parent = parent;
		}
	};
};


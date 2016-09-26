#pragma once
#include "Platform.h"

namespace SupraHot
{
	class Entity;
	class Component
	{
	private:
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

		virtual void Registered() = 0;
		virtual void Destroy() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void LateUpdate(float deltaTime) = 0;
		virtual void FixedUpdate(float deltaTime) = 0;
	};
};


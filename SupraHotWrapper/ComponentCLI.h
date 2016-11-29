#pragma once

#include "WrapperInterface.h"
#include <Component.h>

namespace SupraHot
{
	namespace CLI
	{
		ref class EntityCLI;

		public interface class ComponentCLI : WrapperInterface<SupraHot::Component>
		{
		public:
			virtual void Registered();
			virtual void Removed();
			virtual void Destroy();
			virtual void Update(float deltaTime);
			virtual void LateUpdate(float deltaTime);
			virtual void FixedUpdate(float deltaTime);
			
			virtual void Register(CLI::EntityCLI^ parent);
			virtual void Remove();
			// SupraHot::CLI::EntityCLI^ GetParent();
		};
	};
};
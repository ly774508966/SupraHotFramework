#pragma once

#include "EntityCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		EntityCLI::EntityCLI()
		{
		}

		void EntityCLI::AddComponent(SupraHot::CLI::ComponentCLI^ component)
		{
			printf("Adding component = %p \n", component);
		}
	}
};
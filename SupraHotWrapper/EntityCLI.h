#pragma once
#include "Wrapper.h"
#include "ComponentCLI.h"
#include <Entity.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class EntityCLI : public Wrapper<SupraHot::Entity>
		{
		public:
			EntityCLI();
			~EntityCLI();
			void AddComponent(SupraHot::CLI::ComponentCLI^ component);
			generic <class T>
				where T: ComponentCLI, gcnew()
			T GetComponent();
			System::String^ GetName();
		};
	};
};
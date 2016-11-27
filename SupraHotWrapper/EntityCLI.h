#pragma once
#include "Wrapper.h"
#include <Entity.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Entity : public Wrapper<SupraHot::Entity>
		{
		public:
			Entity();
		};
	};
};
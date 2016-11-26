#pragma once
#include "Platform.h"
#include "Shader.h"

namespace SupraHot
{
	namespace Graphics
	{
		class MaterialProperty
		{
		protected:
			std::string Name;
			uint32 GLLocation;
		public:
			MaterialProperty(std::string name);
			virtual ~MaterialProperty() = 0;
			virtual void Apply(Shader* shader) = 0;
			void SetLocation(Shader* shader);
			std::string GetName();
		};
	};
};
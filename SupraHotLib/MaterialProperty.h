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
			std::string Type = "MaterialProperty";
			uint32 GLLocation = 0;
		public:
			MaterialProperty(std::string name);
			virtual ~MaterialProperty() = 0;
			virtual void Apply(Shader* shader) = 0;
			virtual void Unbind(Shader* shader) = 0;
			void SetLocation(Shader* shader);
			std::string GetName();
			std::string GetType();
		};
	};
};
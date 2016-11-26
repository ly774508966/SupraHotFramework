#pragma once
#include "MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		class BooleanMaterialProperty : public MaterialProperty
		{			
		private:
			bool Value;
		public:
			BooleanMaterialProperty(std::string name);
			~BooleanMaterialProperty();

			void Apply(Shader* shader) override;
			void SetValue(bool value);
			bool GetValue();
		};
	};
};
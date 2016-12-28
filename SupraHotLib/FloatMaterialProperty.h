#pragma once
#include "MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		class FloatMaterialProperty : public MaterialProperty
		{
		private:
			float Value = 0.0f;
		public:
			FloatMaterialProperty(std::string name);
			~FloatMaterialProperty();
			void Apply(Shader* shader) override;
			void SetValue(float value);
			void Unbind(Shader* shader) override;
			float& GetValue();
		};
	};
};
#include "FloatMaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		FloatMaterialProperty::FloatMaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "Float";
			Value = 0.0f;
		}

		FloatMaterialProperty::~FloatMaterialProperty()
		{
		}

		void FloatMaterialProperty::Apply(Shader* shader)
		{
			shader->SetFloat(GLLocation, Value);

		}

		void FloatMaterialProperty::SetValue(float value)
		{
			Value = value;
		}

		float& FloatMaterialProperty::GetValue()
		{
			return Value;
		}
	};
};
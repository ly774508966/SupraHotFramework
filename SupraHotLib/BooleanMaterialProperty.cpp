#include "BooleanMaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		BooleanMaterialProperty::BooleanMaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "Boolean";
			Value = false;
		}
		
		BooleanMaterialProperty::~BooleanMaterialProperty()
		{
		}

		void BooleanMaterialProperty::Apply(Shader* shader)
		{
			shader->SetInteger(GLLocation, Value);
		}

		void BooleanMaterialProperty::SetValue(bool value)
		{
			Value = value;
		}

		bool BooleanMaterialProperty::GetValue()
		{
			return Value;
		}
	};
};
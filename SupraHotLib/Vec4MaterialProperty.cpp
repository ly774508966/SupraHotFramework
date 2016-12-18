#include "Vec4MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		Vec4MaterialProperty::Vec4MaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "Vec4";
		}

		Vec4MaterialProperty::~Vec4MaterialProperty()
		{
		}

		void Vec4MaterialProperty::Apply(Shader* shader)
		{
			shader->SetVec4(GLLocation, Value);
		}

		void Vec4MaterialProperty::SetValue(const Vec4& value)
		{
			this->Value = value;
		}

		Vec4& Vec4MaterialProperty::GetValue()
		{
			return Value;
		}
	};
};
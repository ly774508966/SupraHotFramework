#include "Vec3MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		Vec3MaterialProperty::Vec3MaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "Vec3";
		}

		Vec3MaterialProperty::~Vec3MaterialProperty()
		{

		}

		void Vec3MaterialProperty::Apply(Shader* shader)
		{
			shader->SetVec3(GLLocation, Value);
		}

		void Vec3MaterialProperty::SetValue(const Vec3& value)
		{
			Value = value;
		}

		Vec3& Vec3MaterialProperty::GetValue()
		{
			return Value;
		}
	};
};

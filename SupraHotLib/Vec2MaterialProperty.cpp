#include "Vec2MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		Vec2MaterialProperty::Vec2MaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "Vec2";
		}

		Vec2MaterialProperty::~Vec2MaterialProperty()
		{
		}

		void Vec2MaterialProperty::Apply(Shader* shader)
		{
			shader->SetVec2(GLLocation, Value);
		}

		void Vec2MaterialProperty::SetValue(const Vec2& value)
		{
			Value = value;
		}

		Vec2& Vec2MaterialProperty::GetValue()
		{
			return Value;
		}
	};
};
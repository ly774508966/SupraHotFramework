#include "MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		MaterialProperty::MaterialProperty(std::string name)
		{
			Name = name;
		}

		MaterialProperty::~MaterialProperty()
		{
			
		}

		void MaterialProperty::SetLocation(Shader* shader)
		{
			GLLocation = shader->GetUniformLocation(Name);
		}

		std::string MaterialProperty::GetName()
		{
			return Name;
		}

		std::string MaterialProperty::GetType()
		{
			return Type;
		}
	};
};
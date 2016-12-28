#pragma once
#include "MaterialProperty.h"
#include "Vec4.h"
#include <string>

namespace SupraHot
{
	namespace Graphics
	{
		class Vec4MaterialProperty : public MaterialProperty
		{
		private:
			Vec4 Value;
		public:
			Vec4MaterialProperty(std::string name);
			~Vec4MaterialProperty();
			void Apply(Shader* shader) override;
			void Unbind(Shader* shader) override;
			void SetValue(const Vec4& value);
			Vec4& GetValue();
		};
	};
};
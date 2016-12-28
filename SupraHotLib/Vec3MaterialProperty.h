#pragma once
#include "MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Vec3MaterialProperty : public MaterialProperty
		{
		private:
			Vec3 Value;
		public:
			Vec3MaterialProperty(std::string name);
			~Vec3MaterialProperty();
			void Apply(Shader* shader) override;
			void Unbind(Shader* shader) override;
			void SetValue(const Vec3& value);
			Vec3& GetValue();
		};
	};
};
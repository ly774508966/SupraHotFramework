#pragma once
#include "MaterialProperty.h"
#include <string>

namespace SupraHot
{
	namespace Graphics
	{
		class Vec2MaterialProperty : public MaterialProperty
		{
		private:
			Vec2 Value;
		public:
			Vec2MaterialProperty(std::string name);
			~Vec2MaterialProperty();
			void Apply(Shader* shader) override;
			void SetValue(const Vec2& value);
			Vec2& GetValue();
		};
	};
};
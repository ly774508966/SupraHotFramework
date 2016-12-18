#pragma once
#include "Platform.h"
#include "MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		class TextureCubeMaterialProperty : public MaterialProperty
		{
		private:
			TextureCube* Texture;
			std::string Value;
		public:
			TextureCubeMaterialProperty(std::string name);
			~TextureCubeMaterialProperty();
			void SetValue(std::string pathToTexture);
			std::string GetValue();
			TextureCube* GetTexture();
			void Apply(Shader* shader) override;
		};
	};
};
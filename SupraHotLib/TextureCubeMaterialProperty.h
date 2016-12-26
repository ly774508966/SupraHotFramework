#pragma once
#include "Platform.h"
#include "MaterialProperty.h"
#include "TextureCube.h"

namespace SupraHot
{
	namespace Graphics
	{
		class TextureCubeMaterialProperty : public MaterialProperty
		{
		private:
			TextureCubePtr Texture;
			std::string Value;
		public:
			TextureCubeMaterialProperty(std::string name);
			~TextureCubeMaterialProperty();
			void SetValue(std::string pathToTexture);
			std::string GetValue();
			TextureCubePtr GetTexture();
			void Apply(Shader* shader) override;
		};
	};
};
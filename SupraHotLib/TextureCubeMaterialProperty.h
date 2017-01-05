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
			uint32 TextureSlot;
		public:
			TextureCubeMaterialProperty(std::string name);
			~TextureCubeMaterialProperty();
			void SetValue(std::string pathToTexture);
			void Unbind(Shader* shader) override;
			std::string GetValue();
			TextureCubePtr GetTexture();
			void Apply(Shader* shader) override;

			void Reset();
		};
	};
};
#pragma once
#include "Platform.h"
#include "MaterialProperty.h"
#include "Texture2D.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Texture2DMaterialProperty : public MaterialProperty
		{
		private:
			Texture2DPtr Texture;
			std::string Value;
			uint32 TextureSlot;
		public:
			Texture2DMaterialProperty(std::string name);
			~Texture2DMaterialProperty();
			void SetValue(std::string pathToTexture);
			void Unbind(Shader* shader) override;
			std::string GetValue();
			Texture2DPtr GetTexture();
			void Apply(Shader* shader) override;
		};
	};
};
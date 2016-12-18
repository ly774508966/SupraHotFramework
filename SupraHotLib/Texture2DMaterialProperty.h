#pragma once
#include "Platform.h"
#include "MaterialProperty.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Texture2DMaterialProperty : public MaterialProperty
		{
		private:
			Texture2D* Texture;
			std::string Value;
		public:
			Texture2DMaterialProperty(std::string name);
			~Texture2DMaterialProperty();
			void SetValue(std::string pathToTexture);
			std::string GetValue();
			Texture2D* GetTexture();
			void Apply(Shader* shader) override;
		};
	};
};
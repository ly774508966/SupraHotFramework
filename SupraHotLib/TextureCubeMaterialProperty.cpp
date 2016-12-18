#include "TextureCubeMaterialProperty.h"
#include "TextureCache.h"
#include <cassert>

namespace SupraHot
{
	namespace Graphics
	{
		TextureCubeMaterialProperty::TextureCubeMaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "TextureCube";
		}

		TextureCubeMaterialProperty::~TextureCubeMaterialProperty()
		{
		}

		std::string TextureCubeMaterialProperty::GetValue()
		{
			return Value;
		}

		TextureCube* TextureCubeMaterialProperty::GetTexture()
		{
			return Texture;
		}

		void TextureCubeMaterialProperty::SetValue(std::string pathToTexture)
		{
			// todo: check cached

			if (Value != pathToTexture)
			{
				if (TextureCache::GetInstance()->IsCachedTexture2D(pathToTexture))
				{
					if (TextureCache::GetInstance()->IsCachedTextureCube(Value))
					{
						TextureCache::GetInstance()->FreeAndDeleteTexture(Texture);
						Texture = nullptr;
					}
					else
					{
						Texture->Destroy();
						delete Texture;
						Texture = nullptr;
					}

					Texture = TextureCache::GetInstance()->GetCachedTextureCube(pathToTexture);
					Value = pathToTexture;
				}
			}

		}

		void TextureCubeMaterialProperty::Apply(Shader* shader)
		{
			if (Texture != nullptr)
			{
#if DEVELOPMENT == 1
				assert(shader->LastUsedTextureSlot < GL_TEXTURE31);
#endif

				shader->SetTextureCube(GLLocation, Texture, shader->LastUsedTextureSlot);
				shader->LastUsedTextureSlot = shader->LastUsedTextureSlot + 1;
			}
		}
	};
};

#include "Texture2DMaterialProperty.h"
#include "TextureCache.h"
#include <cassert>

namespace SupraHot
{
	namespace Graphics
	{
		Texture2DMaterialProperty::Texture2DMaterialProperty(std::string name) : MaterialProperty(name)
		{
			Type = "Texture2D";
		}

		Texture2DMaterialProperty::~Texture2DMaterialProperty()
		{
		}

		std::string Texture2DMaterialProperty::GetValue()
		{
			return Value;
		}

		Texture2D* Texture2DMaterialProperty::GetTexture()
		{
			return Texture;
		}

		void Texture2DMaterialProperty::SetValue(std::string pathToTexture)
		{
			// todo: check cached

			if (Value != pathToTexture)
			{
				if (TextureCache::GetInstance()->IsCachedTexture2D(pathToTexture))
				{
					// This is very important.
					// Todo: Check references to this current texture!
					// We could try to solve this with 
					// #1 Reference counted smart pointers,
					// or #2 just use the TextureCache, to count references made to this texture.
					// To ensure, that #2 works, we must inject the TextureCache inside the "Load" function of
					// the Texture2D- & TextureCube-Classes. So that every loaded Texture gets run through it.
					// 
					// The two approaches can be combined together.
					// Also we could create a "Texture"-Loader class, which will only return smartpointer<texture2d>.
					// This way the user can never create (new Texture2D) by himself.


					// Since Textures will be only assigned with the editor, or only through a texture2dmaterialproperty,
					// we can do the reference counting inside the texture cache.

					if (TextureCache::GetInstance()->IsCachedTexture2D(Value))
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

					Texture = TextureCache::GetInstance()->GetCachedTexture2D(pathToTexture);
					Value = pathToTexture;
				}
			}

		}

		void Texture2DMaterialProperty::Apply(Shader* shader)
		{
			// shader->SetTexture2D(GLLocation, Texture, GL_TEXTURE0); 
			if (Texture != nullptr)
			{
#if DEVELOPMENT == 1
				assert(shader->LastUsedTextureSlot < GL_TEXTURE31);
#endif

				shader->SetTexture2D(GLLocation, Texture, shader->LastUsedTextureSlot);
				shader->LastUsedTextureSlot = shader->LastUsedTextureSlot + 1;
			}
		}
	};
};

#include "TextureCubeMaterialProperty.h"
#include "TextureCache.h"
#include <cassert>
#include "FileSystem.h"

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

		TextureCubePtr TextureCubeMaterialProperty::GetTexture()
		{
			return Texture;
		}

		void TextureCubeMaterialProperty::SetValue(std::string pathToTexture)
		{
			// todo: check cached

			// this fails.
			if (Value != pathToTexture)
			{

				// Check if current texture is cached or not.

				if (Texture.get() != nullptr && !TextureCache::GetInstance()->IsCachedTextureCube(Value))
				{
					// FreeTexture() checks, if the texture is not being used anywhere else or not
					TextureCache::GetInstance()->FreeTexture(Texture);
				}


				// Check if the texture we want to use next is already cached
				if (TextureCache::GetInstance()->IsCachedTextureCube(pathToTexture))
				{
					Texture = TextureCache::GetInstance()->GetCachedTextureCube(pathToTexture);
					Value = pathToTexture;
				}
				else
				{

					// dirty hack
					std::string rootPath = Utils::FileSystem::GetInstance()->GetRootPath();
					Utils::FileSystem::GetInstance()->SetRootPath("");

					if (Utils::FileSystem::GetInstance()->FileExists("", pathToTexture))
					{
						TextureCube* rawTexture = new TextureCube(pathToTexture);
						rawTexture->LoadDDS(pathToTexture, true, true);
						SHF_PRINTF("TextureCube @ %s loaded \n", rawTexture->GetPath().c_str());

						TextureCubePtr newTexture(rawTexture);

						TextureCache::GetInstance()->CacheTexture(newTexture);

						Texture = newTexture;
					}
					else
					{
						SHF_PRINTF("Could not load texture, because the given file path could not be resolved \n");
					}

					Utils::FileSystem::GetInstance()->SetRootPath(rootPath);

				}
			}

		}

		void TextureCubeMaterialProperty::Apply(Shader* shader)
		{
			// shader->SetTexture2D(GLLocation, Texture, GL_TEXTURE0); 
			TextureCube* texture = Texture.get();
			if (texture != nullptr)
			{
#if DEVELOPMENT == 1
				static uint32 MAX_TEXTURES = static_cast<uint32>(GL_TEXTURE31);
				assert(shader->LastUsedTextureSlot < MAX_TEXTURES);
#endif

				shader->SetTextureCube(GLLocation, texture, GL_TEXTURE0 + shader->LastUsedTextureSlot);
				shader->LastUsedTextureSlot = shader->LastUsedTextureSlot + 1;
			}
		}
	};
};

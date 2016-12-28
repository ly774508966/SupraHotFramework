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

					if (Utils::FileSystem::GetInstance()->FileExists("", pathToTexture))
					{
						TextureCube* rawTexture = new TextureCube(pathToTexture);
						rawTexture->LoadDDS(pathToTexture, true, true);
						SHF_PRINTF("TextureCube @ %s loaded \n", rawTexture->GetPath().c_str());

						TextureCubePtr newTexture(rawTexture);

						TextureCache::GetInstance()->CacheTexture(newTexture);

						Texture = newTexture;
						return;
					}

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
						Utils::FileSystem::GetInstance()->SetRootPath(rootPath);

						return;
					}

					Utils::FileSystem::GetInstance()->SetRootPath(rootPath);
						
					SHF_PRINTF("Could not load texture, because the given file path could not be resolved \n");
				}
			}

		}

		void TextureCubeMaterialProperty::Unbind(Shader* shader)
		{
		}

		void TextureCubeMaterialProperty::Apply(Shader* shader)
		{
			TextureCube* texture = Texture.get();
			if (texture == nullptr)
			{
				texture = TextureCache::GetInstance()->GetDefaultTextureCube()->get();
			} 

#if DEVELOPMENT == 1
			static uint32 MAX_TEXTURES = static_cast<uint32>(GL_TEXTURE31);
			assert(shader->LastUsedTextureSlot < MAX_TEXTURES);
#endif
			TextureSlot = shader->LastUsedTextureSlot;
			shader->SetTextureCube(GLLocation, texture, GL_TEXTURE0 + TextureSlot);
			shader->LastUsedTextureSlot = shader->LastUsedTextureSlot + 1;
		}
	};
};

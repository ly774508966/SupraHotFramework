#include "Texture2DMaterialProperty.h"
#include "TextureCache.h"
#include <cassert>
#include "FileSystem.h"

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

		Texture2DPtr Texture2DMaterialProperty::GetTexture()
		{
			return Texture;
		}

		void Texture2DMaterialProperty::SetValue(std::string pathToTexture)
		{
			// todo: check cached

			// this fails.
			if (Value != pathToTexture)
			{

				// Check if current texture is cached or not.

				if (Texture.get() != nullptr && !TextureCache::GetInstance()->IsCachedTexture2D(Value))
				{
					// FreeTexture() checks, if the texture is not being used anywhere else or not
					TextureCache::GetInstance()->FreeTexture(Texture);
				}


				// Check if the texture we want to use next is already cached
				if (TextureCache::GetInstance()->IsCachedTexture2D(pathToTexture))
				{
					Texture = TextureCache::GetInstance()->GetCachedTexture2D(pathToTexture);
					Value = pathToTexture;
				}
				else
				{
		
					// dirty hack
					std::string rootPath = Utils::FileSystem::GetInstance()->GetRootPath();
					Utils::FileSystem::GetInstance()->SetRootPath("");

					if (Utils::FileSystem::GetInstance()->FileExists("", pathToTexture))
					{
						Texture2D* rawTexture = new Texture2D(pathToTexture);
						rawTexture->Load(pathToTexture);
						SHF_PRINTF("Texture @ %s loaded \n", rawTexture->GetPath().c_str());

						Texture2DPtr newTexture(rawTexture);

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

		void Texture2DMaterialProperty::Apply(Shader* shader)
		{
			// shader->SetTexture2D(GLLocation, Texture, GL_TEXTURE0); 
			Texture2D* texture = Texture.get();
			if (texture != nullptr)
			{
#if DEVELOPMENT == 1
				static uint32 MAX_TEXTURES = static_cast<uint32>(GL_TEXTURE31);
				assert(shader->LastUsedTextureSlot < MAX_TEXTURES);
#endif

				shader->SetTexture2D(GLLocation, texture, GL_TEXTURE0);
				shader->LastUsedTextureSlot = shader->LastUsedTextureSlot + 1;
			}
		}
	};
};

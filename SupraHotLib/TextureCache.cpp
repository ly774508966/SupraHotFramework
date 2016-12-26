#include "TextureCache.h"

namespace SupraHot
{
	namespace Graphics
	{
		TextureCache::TextureCache()
		{
		}

		TextureCache::~TextureCache()
		{
		}

		bool TextureCache::IsCachedTexture2D(std::string pathToTexture)
		{
			return Texture2Ds.find(pathToTexture) != Texture2Ds.end();
		}

		bool TextureCache::IsCachedTextureCube(std::string pathToTexture)
		{
			return TextureCubes.find(pathToTexture) != TextureCubes.end();
		}

		TextureCache* TextureCache::GetInstance()
		{
			static TextureCache* instance(new TextureCache);
			return instance;
		}

		Texture2DPtr TextureCache::GetCachedTexture2D(std::string pathToTexture)
		{
			return Texture2Ds[pathToTexture];
		}

		TextureCubePtr TextureCache::GetCachedTextureCube(std::string pathToTexture)
		{
			return TextureCubes[pathToTexture];
		}

		void TextureCache::CacheTexture(Texture2DPtr texture2d)
		{
			if (texture2d.get() != nullptr && Texture2Ds.find(texture2d->GetPath()) == Texture2Ds.end())
			{
				Texture2Ds[texture2d->GetPath()] = texture2d;
			}
		}

		void TextureCache::CacheTexture(TextureCubePtr textureCube)
		{
			if (textureCube.get() != nullptr && TextureCubes.find(textureCube->GetPath()) == TextureCubes.end())
			{
				TextureCubes[textureCube->GetPath()] = textureCube;
			}
		}

		void TextureCache::FreeTexture(Texture2DPtr& texture2d)
		{

			SHF_PRINTF("(FreeTexture) Texture count = [%d] \n", texture2d.use_count());

			if (texture2d.get() != nullptr && texture2d.use_count() <= 2 && Texture2Ds.find(texture2d->GetPath()) != Texture2Ds.end())
			{
				Texture2Ds.erase(Texture2Ds.find(texture2d->GetPath()));
			}

		}

		void TextureCache::FreeTexture(TextureCubePtr& textureCube)
		{

			SHF_PRINTF("(FreeTexture) Texture count = [%d] \n", textureCube.use_count());

			if (textureCube.get() != nullptr && textureCube.use_count() <= 2 && TextureCubes.find(textureCube->GetPath()) != TextureCubes.end())
			{
				TextureCubes.erase(TextureCubes.find(textureCube->GetPath()));
			}
		}

		Texture2DPtr TextureCache::WrapTexture(Texture2D* texture2D)
		{
			return Texture2DPtr(texture2D);
		}

		TextureCubePtr TextureCache::WrapTexture(TextureCube* textureCube)
		{
			return TextureCubePtr(textureCube);
		}
	};
};
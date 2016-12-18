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

		Texture2D* TextureCache::GetCachedTexture2D(std::string pathToTexture)
		{
			return Texture2Ds[pathToTexture];
		}

		TextureCube* TextureCache::GetCachedTextureCube(std::string pathToTexture)
		{
			return TextureCubes[pathToTexture];
		}

		void TextureCache::CacheTexture(Texture2D* texture2d)
		{
			if (texture2d != nullptr && Texture2Ds.find(texture2d->GetPath()) == Texture2Ds.end())
			{
				Texture2Ds[texture2d->GetPath()] = texture2d;
			}
		}

		void TextureCache::CacheTexture(TextureCube* textureCube)
		{
			if (textureCube != nullptr && TextureCubes.find(textureCube->GetPath()) == TextureCubes.end())
			{
				TextureCubes[textureCube->GetPath()] = textureCube;
			}
		}

		void TextureCache::FreeTexture(Texture2D* texture2d)
		{
			if (texture2d != nullptr)
			{
				Texture2Ds[texture2d->GetPath()] = nullptr;
			}
		}

		void TextureCache::FreeTexture(TextureCube* textureCube)
		{
			if (textureCube != nullptr)
			{
				TextureCubes[textureCube->GetPath()] = nullptr;
			}
		}

		void TextureCache::FreeAndDeleteTexture(Texture2D* texture2d)
		{
			FreeTexture(texture2d);
			texture2d->Destroy();
			delete texture2d;
		}

		void TextureCache::FreeAndDeleteTexture(TextureCube* textureCube)
		{
			FreeTexture(textureCube);
			textureCube->Destroy();
			delete textureCube;
		}
	};
};
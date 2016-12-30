#include "TextureCache.h"

namespace SupraHot
{
	namespace Graphics
	{
		TextureCache::TextureCache()
		{
		}

		Texture2DPtr* TextureCache::GetDefaultTexture2D()
		{
			return &DefaultTexture2D;
		}

		TextureCubePtr* TextureCache::GetDefaultTextureCube()
		{
			return &DefaultTextureCube;
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

		void TextureCache::Init()
		{
				{
					Texture2D* rawTexture = new Texture2D("Default2D");
					rawTexture->Load("Textures/Default/Default.png");
					DefaultTexture2D = Texture2DPtr(rawTexture);
				}

				{
					TextureCube* rawTexture = new TextureCube("DefaultCube");

					rawTexture->Load(
						"Textures/Default/Default.png", 
						"Textures/Default/Default.png",
						"Textures/Default/Default.png",
						"Textures/Default/Default.png",
						"Textures/Default/Default.png",
						"Textures/Default/Default.png"
					);

					DefaultTextureCube = TextureCubePtr(rawTexture);
				}
		}

		void TextureCache::Destroy()
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("\n- - - - - - - - - - - - \n");
			SHF_PRINTF("TextureCache::Destroy \n\n");
#endif

			DefaultTexture2D.get()->Destroy();
			DefaultTextureCube.get()->Destroy();
			
			{
				typedef std::unordered_map<std::string, Texture2DPtr>::iterator it_type;
				for (it_type iterator = Texture2Ds.begin(); iterator != Texture2Ds.end(); ++iterator)
				{
					iterator->second.get()->Destroy();
				}

				Texture2Ds.clear();
			}

			{
				typedef std::unordered_map<std::string, TextureCubePtr>::iterator it_type;
				for (it_type iterator = TextureCubes.begin(); iterator != TextureCubes.end(); ++iterator)
				{
					iterator->second.get()->Destroy();
				}

				TextureCubes.clear();
			}
#if DEVELOPMENT == 1
			SHF_PRINTF("\nTextureCache::Destroy \n");
			SHF_PRINTF("- - - - - - - - - - - - \n\n");
#endif
		}
	};
};
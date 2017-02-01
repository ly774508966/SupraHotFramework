#pragma once
#include "Platform.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class TextureCache
		{
		private:
			TextureCache();
			
			std::unordered_map<std::string, Texture2DPtr> Texture2Ds;
			std::unordered_map<std::string, TextureCubePtr> TextureCubes;

			Texture2DPtr DefaultTexture2D;
			TextureCubePtr DefaultTextureCube;

			public:

			Texture2DPtr* GetDefaultTexture2D();
			TextureCubePtr* GetDefaultTextureCube();

			~TextureCache();

			static TextureCache* GetInstance();
			void Init();
			void Destroy();
			bool IsCachedTexture2D(std::string pathToTexture);
			bool IsCachedTextureCube(std::string pathToTexture);
			
			Texture2DPtr GetCachedTexture2D(std::string pathToTexture);
			TextureCubePtr GetCachedTextureCube(std::string pathToTexture);

			void CacheTexture(Texture2DPtr texture2d);
			void CacheTexture(TextureCubePtr textureCube);

			void FreeTexture(Texture2DPtr& texture2d);
			void FreeTexture(TextureCubePtr& textureCube);

			Texture2DPtr WrapTexture(Texture2D* texture2D);
			TextureCubePtr WrapTexture(TextureCube* textureCube);
		};
	};
};
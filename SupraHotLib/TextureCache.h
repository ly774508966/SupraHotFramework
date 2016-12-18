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
			
			std::unordered_map<std::string, Texture2D*> Texture2Ds;
			std::unordered_map<std::string, TextureCube*> TextureCubes;

				public:
				~TextureCache();
			static TextureCache* GetInstance();
			bool IsCachedTexture2D(std::string pathToTexture);
			bool IsCachedTextureCube(std::string pathToTexture);
			
			Texture2D* GetCachedTexture2D(std::string pathToTexture);
			TextureCube* GetCachedTextureCube(std::string pathToTexture);

			void CacheTexture(Texture2D* texture2d);
			void CacheTexture(TextureCube* textureCube);

			void FreeTexture(Texture2D* texture2d);
			void FreeTexture(TextureCube* textureCube);

			void FreeAndDeleteTexture(Texture2D* texture2d);
			void FreeAndDeleteTexture(TextureCube* textureCube);
		};
	};
};
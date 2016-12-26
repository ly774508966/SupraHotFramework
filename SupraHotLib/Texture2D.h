#pragma once
#include "Platform.h"
#include <memory>

namespace SupraHot
{
	namespace Graphics
	{
		class Texture2D
		{
		private:
			uint32 TextureID = 0;
			std::string Path;
			std::string Name;
			uint32 Width, Height;

			// Default values
			uint32 WrapS = GL_REPEAT, WrapT = GL_REPEAT;
			uint32 MinFilter = GL_LINEAR_MIPMAP_LINEAR, MagFilter = GL_LINEAR;
			uint32 Format = GL_RGBA, InternalFormat = GL_RGBA8;
			uint32 Type = GL_UNSIGNED_BYTE;

			void LoadWithSTBImage(FILE* f);
			void LoadDDS(FILE* f);
			void LoadDDS(char const * Data, long Size);
		public:
			Texture2D();
			Texture2D(std::string name);
			Texture2D(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT);
			Texture2D(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 minFilter, uint32 magFilter);
			~Texture2D();

			void Load(std::string path);
			void Init(uint32 width, uint32 height);

			void SetName(std::string name);
			void SetWrapS(uint32 wrapS);
			void SetWrapT(uint32 wrapT);
			void SetMinFilter(uint32 minFilter);
			void SetMagFilter(uint32 magFilter);
			void SetInternalFormat(uint32 internalFormat);
			void SetFormat(uint32 format);
			void SetType(uint32 type);

			const std::string& GetName();
			const std::string& GetPath();
			const uint32& GetTextureID();
			const uint32& GetWrapS();
			const uint32& GetWrapT();
			const uint32& GetMinFilter();
			const uint32& GetMagFilter();

			uint32 GetID();

			void Destroy();
		};

		typedef std::shared_ptr<Texture2D> Texture2DPtr;

	};
};
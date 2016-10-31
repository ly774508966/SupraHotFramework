#pragma once
#include "Platform.h"
namespace SupraHot
{
	namespace Graphics
	{
		class TextureCube
		{
		private:
			uint32 TextureID;
			std::string Path;
			std::string Name;
			uint32 Width, Height;

			// Default values
			uint32 WrapS = GL_REPEAT, WrapT = GL_REPEAT, WrapR = GL_REPEAT;
			uint32 MinFilter = GL_LINEAR_MIPMAP_LINEAR, MagFilter = GL_LINEAR;
			uint32 Format = GL_RGBA, InternalFormat = GL_RGBA8;
			uint32 Type = GL_UNSIGNED_BYTE;

			void LoadDDS(char const* Data, long Size, bool checkIsCube);
		public:
			TextureCube();
			TextureCube(std::string name);
			TextureCube(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 wrapR);
			TextureCube(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 wrapR, uint32 minFilter, uint32 magFilter);
			~TextureCube();

			void SetName(std::string name);
			void SetWrapS(uint32 wrapS);
			void SetWrapT(uint32 wrapT);
			void SetWrapR(uint32 wrapR);
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
			const uint32& GetWrapR();
			const uint32& GetMinFilter();
			const uint32& GetMagFilter();

			uint32 GetID();

			void Destroy();

			void Init(uint32 width, uint32 height);
			void LoadDDS(std::string path, bool checkIsCube);
			void Load(std::string left, std::string right, std::string top, std::string bottom, std::string front, std::string back);
		};
	}
}


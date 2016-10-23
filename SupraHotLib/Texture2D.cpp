#include "Texture2D.h"
#include "stb_image.h"
#include "FileSystem.h"
#include <stdio.h>
#include "StringUtil.h"
#include "DDSUtil.h"
#include <cassert>

namespace SupraHot
{
	namespace Graphics
	{
		Texture2D::Texture2D()
		{
			this->TextureID = 0;
		}

		Texture2D::Texture2D(std::string name)
		{
			this->TextureID = 0;
			this->Name = name;
		}

		Texture2D::Texture2D(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT)
		{
			this->TextureID = 0;
			this->Format = format;
			this->InternalFormat = internalFormat;
			this->Type = type;
			this->WrapS = wrapS;
			this->WrapT = wrapT;
		}

		Texture2D::Texture2D(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 minFilter, uint32 magFilter)
		{
			this->TextureID = 0;
			this->Format = format;
			this->InternalFormat = internalFormat;
			this->Type = type;
			this->WrapS = wrapS;
			this->WrapT = wrapT;
			this->MinFilter = minFilter;
			this->MagFilter = magFilter;
		}

		Texture2D::~Texture2D()
		{
		}

		void Texture2D::Load(std::string path)
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("Loading Texture2D : %s \n", path.c_str());
#endif
			this->Path = path;

			/* create and enable texture buffer */
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			/* set texture parameters */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);

			FILE* f = Utils::FileSystem::GetInstance()->GetFile("", Path, "rb");

			if (f == nullptr)
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("Could't load Texture2D. Failed to open file at '%s' \n", path.c_str());
#endif
				return;
			}

			std::string fileExtension = Utils::StringUtil::GetFileExtension(Path);
	
			if (fileExtension == "dds" || fileExtension == "DDS")
			{
				// Load .dds

				LoadDDS(f);

				// File is being closed inside function
			}
			else
			{
				// Load with stbi

				LoadWithSTBImage(f);

				// File is being closed inside function
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture2D::Init(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;

			/* create an enable texture buffer */
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			/* bind data to buffer*/
			glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, Type, nullptr);

			/* set texture parameters */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);

			/* unbind texture */
			glBindTexture(GL_TEXTURE_2D, 0);

#if DEVELOPMENT == 1
			int err = glGetError();
			if (err != 0)
			{
				SHF_PRINTF("Error %d happened while initializing Texture2D (%s) \n", err, Name.c_str());
			}
#endif
		}

		void Texture2D::SetName(std::string name)
		{
			this->Name = name;
		}

		void Texture2D::SetWrapS(uint32 wrapS)
		{
			this->WrapS = wrapS;
		}

		void Texture2D::SetWrapT(uint32 wrapT)
		{
			this->WrapT = wrapT;
		}

		void Texture2D::SetMinFilter(uint32 minFilter)
		{
			this->MinFilter = minFilter;
		}

		void Texture2D::SetInternalFormat(uint32 internalFormat)
		{
			this->InternalFormat = internalFormat;
		}

		void Texture2D::SetFormat(uint32 format)
		{
			this->Format = format;
		}

		void Texture2D::SetType(uint32 type)
		{
			this->Type = type;
		}

		void Texture2D::SetMagFilter(uint32 magFilter)
		{
			this->MagFilter = magFilter;
		}

		const std::string& Texture2D::GetName()
		{
			return Name;
		}

		const std::string& Texture2D::GetPath()
		{
			return Path;
		}

		const uint32& Texture2D::GetTextureID()
		{
			return TextureID;
		}

		const uint32& Texture2D::GetWrapS()
		{
			return WrapS;
		}

		const uint32& Texture2D::GetWrapT()
		{
			return WrapT;
		}

		const uint32& Texture2D::GetMinFilter()
		{
			return MinFilter;
		}

		const uint32& Texture2D::GetMagFilter()
		{
			return MagFilter;
		}

		uint32 Texture2D::GetID()
		{
			return TextureID;
		}

		void Texture2D::Destroy()
		{
			glDeleteTextures(1, &TextureID);
			TextureID = 0;

#if DEVELOPMENT == 1
			SHF_PRINTF("Texture %s destroyed \n", this->Name.c_str());
#endif
		}

		void Texture2D::LoadWithSTBImage(FILE* f)
		{
			/* load texture with stb_image */
			int width, height, n;
			unsigned char * data = stbi_load_from_file(f, &width, &height, &n, 0);
			std::fclose(f);

			this->Width = width;
			this->Height = height;

			if (!data)
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("Error, while loading texture (%s): %s\n", Name.c_str(), stbi_failure_reason());
#endif
				glDeleteTextures(1, &TextureID);
			}
			else
			{
				/* Invert Texture on Y Axis */
				for (int j = 0; j * 2 < height; ++j)
				{
					int index1 = j * width * n;
					int index2 = (height - 1 - j) * width * n;
					for (int i = width * n; i > 0; --i)
					{
						unsigned char temp = data[index1];
						data[index1] = data[index2];
						data[index2] = temp;
						++index1;
						++index2;
					}
				}

				/* bind texture to buffer */
				glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, Type, &data[0]);

				/* generate mipmaps */
				glGenerateMipmap(static_cast<GLenum>(GL_TEXTURE_2D));

#if DEVELOPMENT == 1
				int err = glGetError();
				if (err != 0)
				{
					SHF_PRINTF("Error %d happened while loading Texture 2D \n", err);
				}
#endif

				/* free stbi buffer */
				stbi_image_free(data);

#if DEVELOPMENT == 1
				SHF_PRINTF("Loaded Texture2DGL: %s [%d X %d (%d Channels)] #%d \n", Path.c_str(), width, height, n, TextureID);
#endif

				glBindTexture(GL_TEXTURE_2D, 0);
				return;
			}

#if DEVELOPMENT == 1
			SHF_PRINTF("Failed to load Texture: %s \n", Path.c_str());
#endif
		}

		void Texture2D::LoadDDS(FILE* f)
		{
			
			long beg = std::ftell(f);
			std::fseek(f, 0, SEEK_END);

			long end = std::ftell(f);
			std::fseek(f, 0, SEEK_SET);

			std::vector<char> data(static_cast<long>(end - beg));
			std::fread(&data[0], 1, data.size(), f);
			std::fclose(f);

			LoadDDS(&data[0], static_cast<long>(data.size()));
		}

		void Texture2D::LoadDDS(char const* Data, long Size)
		{

#if DEVELOPMENT == 1
			assert(Data && (Size >= sizeof(Utils::DDSUtil::ddsHeader)));
#endif

			Utils::DDSUtil::ddsHeader const & header(*reinterpret_cast<Utils::DDSUtil::ddsHeader const *>(Data));

			if (strncmp(header.Magic, "DDS ", 4) != 0)
			{
				glDeleteTextures(1, &TextureID);
				return;
			}

			size_t Offset = sizeof(Utils::DDSUtil::ddsHeader);

			Utils::DDSUtil::ddsHeader10 header10;
			if (header.Format.flags & Utils::DDSUtil::DDPF_FOURCC && header.Format.fourCC == Utils::DDSUtil::D3DFMT_DX10)
			{
				std::memcpy(&header10, Data + Offset, sizeof(header10));
				Offset += sizeof(Utils::DDSUtil::ddsHeader10);
				SHF_PRINTF("is header10 \n");
			}

			uint32 formatColorComponents;
			uint32 formatSize = 0;
			bool isGL_BGRA = false;
			SHF_PRINTF("Loading DDS with %d bpp \n", header.Format.bpp);

			if ((header.Format.flags 
				& (Utils::DDSUtil::DDPF_RGB 
				| Utils::DDSUtil::DDPF_ALPHAPIXELS 
				| Utils::DDSUtil::DDPF_ALPHA 
				| Utils::DDSUtil::DDPF_YUV 
				| Utils::DDSUtil::DDPF_LUMINANCE)) 
				&& header.Format.flags != Utils::DDSUtil::DDPF_FOURCC_ALPHAPIXELS)
			{
				switch (header.Format.bpp)
				{
				case 8:
				{
					break;
				}
				case 16:
				{
					break;
				}
				case 24:
				{
					// Todo: figure this out.
					formatColorComponents = 3;

					if (header.Format.Mask.x == 0x0000FF &&
						header.Format.Mask.y == 0x00FF00 &&
						header.Format.Mask.z == 0xFF0000)
					{
						// RGB
						Format = GL_RGB;
						InternalFormat = GL_RGB8;
						formatSize = (sizeof(uint32) / 4) * formatColorComponents;
					}
					break;
				}
				case 32:
				{
					formatColorComponents = 4;

					if (header.Format.Mask.x == 0x000000FF &&
						header.Format.Mask.y == 0x0000FF00 &&
						header.Format.Mask.z == 0x00FF0000 &&
						header.Format.Mask.w == 0x00000000)
					{
						// RGB
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						formatSize = sizeof(uint32);
					}
					else if(header.Format.Mask.x == 0x00FF0000 &&
							header.Format.Mask.y == 0x0000FF00 &&
							header.Format.Mask.z == 0x000000FF &&
							header.Format.Mask.w == 0x00000000)
					{
						// BGR
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						isGL_BGRA = true;
						formatSize = sizeof(uint32);
					}
					else if(header.Format.Mask.x == 0x00FF0000 &&
							header.Format.Mask.y == 0x0000FF00 &&
							header.Format.Mask.z == 0x000000FF &&
							header.Format.Mask.w == 0xFF000000)
					{
						// BGRA
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						isGL_BGRA = true;
						formatSize = sizeof(uint32);
					}
					else if(header.Format.Mask.x == 0x000000FF &&
							header.Format.Mask.y == 0x0000FF00 &&
							header.Format.Mask.z == 0x00FF0000 &&
							header.Format.Mask.w == 0xFF000000)
					{
						// RGBA
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						formatSize = sizeof(uint32);
					}
				}
				break;
				}
			}
			else if ((header.Format.fourCC == Utils::DDSUtil::D3DFMT_DX10) && (header10.Format != Utils::DDSUtil::DXGI_FORMAT_UNKNOWN))
			{
				SHF_PRINTF("Find the right format for the header10 \n");

				// here we need to set the right decompression shit to load the files.

				// todo: load compressed files.....
			}


			uint32 mipMapCount = (header.Flags & Utils::DDSUtil::ddsFlag::DDSD_MIPMAPCOUNT) ? header.MipMapLevels : 1;
			uint32 faceCount = (header.CubemapFlags & Utils::DDSUtil::ddsCubemapflag::DDSCAPS2_CUBEMAP) ? 6 : 1;
			uint32 depthCount = (header.CubemapFlags & Utils::DDSUtil::ddsCubemapflag::DDSCAPS2_VOLUME) ? header.Depth : 1;
		
			uint64 totalBufferOffset = 0;
			uint64 bufferElements = 0;
			uint64 bufferSize = 0;

			for (uint32 mip = 0; mip < mipMapCount; mip++)
			{
				uint32 divider = static_cast<uint32>(pow(2, mip));
				uint32 targetWidth = header.Width / divider;
				uint32 targetHeight = header.Height / divider;
				bufferElements = (targetWidth * targetHeight * faceCount);
				bufferSize = bufferElements * formatSize;

				void* buffer = new void*[bufferElements];
				std::memcpy(buffer, (Data + Offset + totalBufferOffset), bufferSize);
				totalBufferOffset += bufferSize;

				if (isGL_BGRA)	// check if BGRA(uint32 per pixel)
				{
					// This converts the format GL_BGRA to GL_RGBA on the fly for each pixel.
					uint32* u32Buffer = static_cast<uint32*>(buffer);
					for (uint64 pixel = 0; pixel < bufferElements; pixel++)
					{
						uint32 comp = u32Buffer[pixel];

						uint32 r = (comp & 0x000000FF);
						uint32 g = (comp & 0x0000FF00) >> 8;
						uint32 b = (comp & 0x00FF0000) >> 16;
						uint32 a = (comp & 0xFF000000) >> 24;

						u32Buffer[pixel] = (a << 24) + (r << 16) + (g << 8) + b;
					}
				}

				// Flip the texture on X-axis here.	
				// Source: https://github.com/paroj/nv_dds/blob/master/nv_dds.cpp
				{
					uint64 imagesize = bufferSize / depthCount;
					uint64 linesize = imagesize / targetHeight;
					uint64 offset;

					uint8 *tmp = new uint8[linesize];

					for (uint32 n = 0; n < depthCount; n++) 
					{
						offset = imagesize * n;
						uint8 *top = (uint8*)buffer + offset;
						uint8 *bottom = top + (imagesize - linesize);

						for (uint32 i = 0; i < (targetHeight >> 1); i++) 
						{
							// swap
							std::memcpy(tmp, bottom, linesize);
							std::memcpy(bottom, top, linesize);
							std::memcpy(top, tmp, linesize);

							top += linesize;
							bottom -= linesize;
						}
					}

					delete[] tmp;
				}

				// todo: check if 32f or 16f.

				SHF_PRINTF("Mip: %d | [%d x %d] \n", mip, targetWidth, targetHeight);
				glTexImage2D(GL_TEXTURE_2D, mip, InternalFormat, targetWidth, targetHeight, 0, Format, Type, buffer);

				// need to free the memory.
				delete[] buffer;
			}
		}
	};
};
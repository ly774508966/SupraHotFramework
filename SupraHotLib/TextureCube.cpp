#include "TextureCube.h"
#include "StringUtil.h"
#include "stb_image.h"
#include "FileSystem.h"
#include "DDSUtil.h"
#include <cassert>

namespace SupraHot
{
	namespace Graphics
	{
		TextureCube::TextureCube()
		{
			this->TextureID = 0;
		}

		TextureCube::TextureCube(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 wrapR)
		{
			this->TextureID = 0;
			this->Format = format;
			this->InternalFormat = internalFormat;
			this->Type = type;
			this->WrapS = wrapS;
			this->WrapT = wrapT;
			this->WrapR = wrapR;
		}

		TextureCube::TextureCube(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 wrapR, uint32 minFilter, uint32 magFilter)
		{
			this->TextureID = 0;
			this->Format = format;
			this->InternalFormat = internalFormat;
			this->Type = type;
			this->WrapS = wrapS;
			this->WrapT = wrapT;
			this->WrapR = wrapR;
			this->MinFilter = minFilter;
			this->MagFilter = magFilter;
		}

		TextureCube::TextureCube(std::string name)
		{
			this->TextureID = 0;
			this->Name = name;
		}

		TextureCube::~TextureCube()
		{
			//Todo: We are not allowed to derefence tex pointers!
			SHF_PRINTF("Triggered destructor of %s (TextureCube)\n", Name.c_str());
			Destroy();
		}

		void TextureCube::SetName(std::string name)
		{
			this->Name = name;
		}

		void TextureCube::SetWrapS(uint32 wrapS)
		{
			this->WrapS = wrapS;
		}

		void TextureCube::SetWrapT(uint32 wrapT)
		{
			this->WrapT = wrapT;
		}

		void TextureCube::SetWrapR(uint32 wrapR)
		{
			this->WrapR = wrapR;
		}

		void TextureCube::SetMinFilter(uint32 minFilter)
		{
			this->MinFilter = minFilter;
		}

		void TextureCube::SetMagFilter(uint32 magFilter)
		{
			this->MagFilter = magFilter;
		}

		void TextureCube::SetInternalFormat(uint32 internalFormat)
		{
			this->InternalFormat = internalFormat;
		}

		void TextureCube::SetFormat(uint32 format)
		{
			this->Format = format;
		}

		void TextureCube::SetType(uint32 type)
		{
			this->Type = type;
		}

		const std::string& TextureCube::GetName()
		{
			return Name;
		}

		const std::string& TextureCube::GetPath()
		{
			return Path;
		}

		const uint32& TextureCube::GetTextureID()
		{
			return TextureID;
		}

		const uint32& TextureCube::GetWrapS()
		{
			return WrapS;
		}

		const uint32& TextureCube::GetWrapT()
		{
			return WrapT;
		}

		const uint32& TextureCube::GetWrapR()
		{
			return WrapR;
		}

		const uint32& TextureCube::GetMinFilter()
		{
			return MinFilter;
		}

		const uint32& TextureCube::GetMagFilter()
		{
			return MagFilter;
		}

		uint32 TextureCube::GetID()
		{
			return TextureID;
		}

		void TextureCube::Destroy()
		{
			if (TextureID != 0)
			{
				glDeleteTextures(1, &TextureID);
				TextureID = 0;
			}

#if DEVELOPMENT == 1
			SHF_PRINTF("Texture %s destroyed \n", this->Name.c_str());
#endif
		}

		void TextureCube::Init(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;

			/* create an enable texture buffer */
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

			for (uint32 i = 0; i < 6; i++)
			{
				/* bind data to buffer*/
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InternalFormat, width, height, 0, Format, Type, nullptr);

				/* set texture parameters */
#ifdef PLATFORM_WINDOWS
				glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#else
				glEnable(GL_TEXTURE_CUBE_MAP);
#endif

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, MinFilter);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, MagFilter);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR);
			}

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

#if DEVELOPMENT == 1
			int err = glGetError();
			if (err != 0)
			{
				SHF_PRINTF("error: %d ,failed to create mipmaps for cube_map \n", err);
			}
#endif

			/* unbind texture */
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

#if DEVELOPMENT == 1
			err = glGetError();
			if (err != 0)
			{
				SHF_PRINTF("Error %d happened while initializing Texture2D \n", err);
			}
#endif
		}

		
		void TextureCube::LoadDDS(std::string path, bool checkIsCube, bool flipX)
		{
			this->Path = path;
			
			FILE* f = Utils::FileSystem::GetInstance()->GetFile("", Path, "rb");

			if (f != nullptr)
			{
				std::string fileExtension = Utils::StringUtil::GetFileExtension(path);

				if (fileExtension == "dds" || fileExtension == "DDS")
				{
					// File is being closed inside function

					long beg = std::ftell(f);
					std::fseek(f, 0, SEEK_END);

					long end = std::ftell(f);
					std::fseek(f, 0, SEEK_SET);

					std::vector<char> data(static_cast<long>(end - beg));
					std::fread(&data[0], 1, data.size(), f);
					std::fclose(f);

					LoadDDS(&data[0], static_cast<long>(data.size()), checkIsCube, flipX);
					
					data.clear();
				} 
				else
				{
					std::fclose(f);
				}
			}
		}

		void TextureCube::LoadDDS(char const* Data, long Size, bool checkIsCube, bool flipX)
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("Loading DDS %s \n", Path.c_str());
#endif

#if DEVELOPMENT == 1
			assert(Data && (Size >= sizeof(Utils::DDSUtil::ddsHeader)));
#endif

			Utils::DDSUtil::ddsHeader const & header(*reinterpret_cast<Utils::DDSUtil::ddsHeader const *>(Data));

			if (strncmp(header.Magic, "DDS ", 4) != 0)
			{
				return;
			}

			size_t Offset = sizeof(Utils::DDSUtil::ddsHeader);

			Utils::DDSUtil::ddsHeader10 header10;
			if (header.Format.flags & Utils::DDSUtil::DDPF_FOURCC && header.Format.fourCC == Utils::DDSUtil::D3DFMT_DX10)
			{
				std::memcpy(&header10, Data + Offset, sizeof(header10));
				Offset += sizeof(Utils::DDSUtil::ddsHeader10);
			}

			uint32 formatComponents = 1;
			uint32 formatSize = 0;
			bool isGL_BGRA = false;

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
					formatComponents = 1;

					if (header.Format.Mask.x == 0x000000FF &&
						header.Format.Mask.y == 0x0000FF00 &&
						header.Format.Mask.z == 0x00FF0000)
					{
						// RGB
						Format = GL_RGB;
						InternalFormat = GL_RGB8;
						formatSize = sizeof(uint32);
					}

					SHF_PRINTF("Not supported yet. \n");
					break;
				}
				case 32:
				{
					if (header.Format.Mask.x == 0x000000FF &&
						header.Format.Mask.y == 0x0000FF00 &&
						header.Format.Mask.z == 0x00FF0000 &&
						header.Format.Mask.w == 0x00000000)
					{
						// RGB
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						formatSize = sizeof(uint8);
						formatComponents = 4;
					}
					else if (header.Format.Mask.x == 0x00FF0000 &&
						header.Format.Mask.y == 0x0000FF00 &&
						header.Format.Mask.z == 0x000000FF &&
						header.Format.Mask.w == 0x00000000)
					{
						// BGRload
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						isGL_BGRA = true;
						formatSize = sizeof(uint8);
						formatComponents = 4;
					}
					else if (header.Format.Mask.x == 0x00FF0000 &&
						header.Format.Mask.y == 0x0000FF00 &&
						header.Format.Mask.z == 0x000000FF &&
						header.Format.Mask.w == 0xFF000000)
					{
						// BGRA
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						isGL_BGRA = true;
						formatSize = sizeof(uint8);
						formatComponents = 4;
					}
					else if (header.Format.Mask.x == 0x000000FF &&
						header.Format.Mask.y == 0x0000FF00 &&
						header.Format.Mask.z == 0x00FF0000 &&
						header.Format.Mask.w == 0xFF000000)
					{
						// RGBA
						Format = GL_RGBA;
						InternalFormat = GL_RGBA8;
						formatSize = sizeof(uint8);
						formatComponents = 4;
					}
				}
				break;
				}
			}
			else if (header.Format.fourCC == Utils::DDSUtil::D3DFMT_A32B32G32R32F)
			{
				// RGBA
				Format = GL_RGBA;
				InternalFormat = GL_RGBA32F;
				Type = GL_FLOAT;
				formatSize = sizeof(float);
				formatComponents = 4;
			}
			else if (header.Format.fourCC == Utils::DDSUtil::D3DFMT_A16B16G16R16F)
			{
				// RGBA
				Format = GL_RGBA;
				InternalFormat = GL_RGBA16F;
				Type = GL_HALF_FLOAT;
				formatSize = sizeof(float) / 2;
				formatComponents = 4;
			}
			else if (header.Format.fourCC == Utils::DDSUtil::D3DFMT_A16B16G16R16)
			{
				// RGBA
				// Todo: this does not work on android. 
				// todo: figure this out.
#ifdef PLATFORM_ANDROID
				Format = GL_RGBA_INTEGER;
				InternalFormat = GL_RGBA16UI;
				Type = GL_UNSIGNED_SHORT;
				formatSize = sizeof(uint16);
				formatComponents = 4;

#if DEVELOPMENT == 1
				SHF_PRINTF("Format not supported on Android! \n");
#endif

#else
				Format = GL_RGBA;
				InternalFormat = GL_RGBA16;
				Type = GL_UNSIGNED_SHORT;
				formatSize = sizeof(uint16);
				formatComponents = 4;
#endif
			}
			else if ((header.Format.fourCC == Utils::DDSUtil::D3DFMT_DX10) && (header10.Format != Utils::DDSUtil::DXGI_FORMAT_UNKNOWN))
			{
				if (header10.Format == Utils::DDSUtil::DXGI_FORMAT_R32G32B32A32_FLOAT)
				{
					// RGBA
					Format = GL_RGBA;
					InternalFormat = GL_RGBA32F;
					Type = GL_FLOAT;
					formatSize = sizeof(float);
					formatComponents = 4;
				} 
				else if (header10.Format == Utils::DDSUtil::DXGI_FORMAT_R16G16B16A16_FLOAT)
				{
					// RGBA
					Format = GL_RGBA;
					InternalFormat = GL_RGBA16F;
					Type = GL_HALF_FLOAT;
					formatSize = sizeof(float) / 2;
					formatComponents = 4;
				} 
				else if (header10.Format == Utils::DDSUtil::DXGI_FORMAT_R8G8B8A8_UNORM
						|| header10.Format == Utils::DDSUtil::DXGI_FORMAT_R8G8B8A8_TYPELESS
						|| header10.Format == Utils::DDSUtil::DXGI_FORMAT_R8G8B8A8_UINT)
				{
					Format = GL_RGBA;
					InternalFormat = GL_RGBA8;
					Type = GL_UNSIGNED_BYTE;
					formatSize = sizeof(uint8);
					formatComponents = 4;
				}
				else if (header10.Format == Utils::DDSUtil::DXGI_FORMAT_B8G8R8A8_UNORM
						|| header10.Format == Utils::DDSUtil::DXGI_FORMAT_B8G8R8A8_TYPELESS
						|| header10.Format == Utils::DDSUtil::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
				{
					Format = GL_RGBA;
					InternalFormat = GL_RGBA8;
					Type = GL_UNSIGNED_BYTE;
					formatSize = sizeof(uint8);
					formatComponents = 4;
					isGL_BGRA = true;
				}
				else if (header10.Format == Utils::DDSUtil::D3DFMT_A8B8G8R8)
				{
					Format = GL_RGBA;
					InternalFormat = GL_RGBA8;
					Type = GL_UNSIGNED_BYTE;
					formatSize = sizeof(uint8);
					formatComponents = 4;
					isGL_BGRA = true;
				}
				else if (header10.Format == Utils::DDSUtil::D3DFMT_R8G8B8)
				{
					Format = GL_RGBA;
					InternalFormat = GL_RGBA8;
					formatSize = sizeof(uint8);
					formatComponents = 4;
				} 
				else
				{
#if DEVELOPMENT == 1
					SHF_PRINTF("Format not supported! Header = %d \n", header10.Format);
#endif
					return;
				}
			}

			uint32 mipMapCount = (header.Flags & Utils::DDSUtil::ddsFlag::DDSD_MIPMAPCOUNT) ? header.MipMapLevels : 1;
			uint32 faceCount = (header.CubemapFlags & Utils::DDSUtil::ddsCubemapflag::DDSCAPS2_CUBEMAP) ? 6 : 1;
			uint32 depthCount = (header.CubemapFlags & Utils::DDSUtil::ddsCubemapflag::DDSCAPS2_VOLUME) ? header.Depth : 1;

			if (faceCount == 1 && checkIsCube)
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("Supplied dds file seems to be not a cube map!\n %s.\n", this->Path.c_str());
#endif
			} 
			else
			{
				faceCount = 6;
			}


			/* create and enable texture buffer */
			glGenTextures(1, &TextureID);

#ifdef PLATFORM_WINDOWS
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#else
			glEnable(GL_TEXTURE_CUBE_MAP);
#endif

			glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

			/* set texture parameters */
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, MagFilter);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, MinFilter);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR);


			uint64 totalBufferOffset = 0;
			uint64 bufferElements = 0;
			uint64 bufferSize = 0;

			for (uint32 f = 0; f < faceCount; ++f)
			{
				// Read the texture data here
				for (uint32 mip = 0; mip < mipMapCount; ++mip)
				{
					uint32 divider = static_cast<uint32>(pow(2, mip));
					uint32 targetWidth = header.Width / divider;
					uint32 targetHeight = header.Height / divider;
					bufferElements = (targetWidth * targetHeight) * formatComponents;
					bufferSize = bufferElements * formatSize;

					void* buffer = new void*[bufferElements];
					std::memcpy(buffer, (Data + Offset + totalBufferOffset), bufferSize);
					totalBufferOffset += bufferSize;

					if (isGL_BGRA)
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

					// Todo: mirror image on y axis. (from left to right)
					if(flipX) {
						uint64 imagesize = bufferSize / depthCount;
						uint64 linesize = imagesize / targetHeight;

						uint8 *top = (uint8*)buffer;
						uint64 pixelSize = formatComponents * formatSize;
						void* swapPixel = malloc(pixelSize);

						for (uint32 y = 0; y < targetHeight; y++)
						{
							
							for (uint32 x = 0; x < targetWidth / 2; x++)
							{
								uint8* leftMarker = top + (x * pixelSize);
								uint8* rightMarker = top + ((targetWidth - 1 - x) * pixelSize);
							
								std::memcpy(swapPixel, leftMarker, pixelSize);
								std::memcpy(leftMarker, rightMarker, pixelSize);
								std::memcpy(rightMarker, swapPixel, pixelSize);
							}

							top += linesize;
						}
						
						free(swapPixel);
					}

#if DEVELOPMENT == 1
					SHF_PRINTF("Face: %d | Mip: %d | [%d x %d] \n", f, mip, targetWidth, targetHeight);
#endif

					uint32 targetFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X + f;
					if (flipX)
					{
						if (f == 0)
						{
							targetFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
						}
						else if (f == 1)
						{
							targetFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
						}
					}

					glTexImage2D(targetFace, mip, InternalFormat, targetWidth, targetHeight, 0, Format, Type, buffer);

					// need to free the memory.
					delete[] buffer;
				}

				// End reading texture data
			}

			if (mipMapCount == 1)
			{
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		void TextureCube::Load(std::string left, std::string right, std::string top, std::string bottom, std::string front, std::string back)
		{
			this->Path = left + " | " + right + " | " + top + " | " + bottom + " | " + front + " | " + back;

			/* create and enable texture buffer */
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

#ifndef PLATFORM_EMSCRIPTEN
			std::vector<std::string> paths = { left, right, top, bottom, front, back };
#else
			std::vector<std::string> paths;
			paths.push_back(left);
			paths.push_back(right);
			paths.push_back(top);
			paths.push_back(bottom);
			paths.push_back(front);
			paths.push_back(back);
#endif
			for (uint32 f = 0; f < 6; f++)
			{
				std::string path = paths[f];

				/* load texture with stb_image */
				int width, height, n;

				FILE* file = Utils::FileSystem::GetInstance()->GetFile("", path, "rb");
				unsigned char * data = stbi_load_from_file(file, &width, &height, &n, 0);
				std::fclose(file);

				if (!data)
				{
#if DEVELOPMENT == 1
					SHF_PRINTF("Error, while loading texture: %s\n", stbi_failure_reason());
#endif
				}
				else
				{
					if (f == 3)
					{
						// flip on y axis
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

						// todo: rework this
						// flip on x axis
					}

#ifdef PLATFORM_WINDOWS
					glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#else
					glEnable(GL_TEXTURE_CUBE_MAP);
#endif
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, MagFilter);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, MinFilter);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR);

					/* bind texture to buffer */
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, 0, InternalFormat, width, height, 0, Format, Type, &data[0]);

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
					SHF_PRINTF("Loaded TextureCube Part: %s [%d X %d (%d Channels)] #%d \n", path.c_str(), width, height, n, TextureID);
#endif
				}
			}

			/* generate mipmaps */
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

#if DEVELOPMENT == 1
			int err = glGetError();
			if (err != 0)
			{
				SHF_PRINTF("error: %d ,failed to create mipmaps for cube_map \n", err);
			}
#endif

#if DEVELOPMENT == 1
			SHF_PRINTF("Loaded CubeMap with mipmaps \n");
#endif
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}
}

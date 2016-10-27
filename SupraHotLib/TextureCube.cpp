#include "TextureCube.h"
#include "StringUtil.h"
#include "stb_image.h"
#include "FileSystem.h"

namespace SupraHot
{
	namespace Graphics
	{
		TextureCube::TextureCube()
		{
		}

		TextureCube::TextureCube(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 wrapR)
		{
			this->Format = format;
			this->InternalFormat = internalFormat;
			this->Type = type;
			this->WrapS = wrapS;
			this->WrapT = wrapT;
			this->WrapR = wrapR;
		}

		TextureCube::TextureCube(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 wrapR, uint32 minFilter, uint32 magFilter)
		{
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
			this->Name = name;
		}


		TextureCube::~TextureCube()
		{
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
			glDeleteTextures(1, &TextureID);
			TextureID = 0;

#if DEVELOPMENT == 1
			printf("Texture %s destroyed \n", this->Name.c_str());
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
				printf("error: %d ,failed to create mipmaps for cube_map \n", err);
			}
#endif

			/* unbind texture */
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

#if DEVELOPMENT == 1
			err = glGetError();
			if (err != 0)
			{
				printf("Error %d happened while initializing Texture2D \n", err);
			}
#endif
		}

		
		void TextureCube::LoadDDS(std::string path)
		{
			// TODO: Load dds cube map
		}

		void TextureCube::Load(std::string left, std::string right, std::string top, std::string bottom, std::string front, std::string back)
		{
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

						/*
						// todo: rework this
						// flip on x axis
						unsigned char * data2 = new unsigned char[width * height * n];
						for (int y = 0; y < height; y++)
						{
							for (int x = 0; x < width; x++)
							{
								uint32 lastElement = (y * width * n) + (width * n);
								uint32 currentElement = y * width * n;
								data2[currentElement + x * n + 0] = data[lastElement - x * n - 0];
								data2[currentElement + x * n + 1] = data[lastElement - x * n - 3];
								data2[currentElement + x * n + 2] = data[lastElement - x * n - 2];
								data2[currentElement + x * n + 3] = data[lastElement - x * n - 1];
							}
						}

						data = data2;
						*/
					}



#ifdef PLATFORM_WINDOWS
					//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#else
					glEnable(GL_TEXTURE_CUBE_MAP);
#endif
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, MagFilter);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, MinFilter);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT);

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

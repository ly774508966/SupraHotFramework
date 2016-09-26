#include "Texture2D.h"
#include "stb_image.h"

namespace SupraHot
{
	namespace Graphics
	{
		Texture2D::Texture2D()
		{

		}

		Texture2D::Texture2D(std::string name)
		{
			this->Name = name;
		}

		Texture2D::Texture2D(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT)
		{
			this->Format = format;
			this->InternalFormat = internalFormat;
			this->Type = type;
			this->WrapS = wrapS;
			this->WrapT = wrapT;
		}

		Texture2D::Texture2D(uint32 format, uint32 internalFormat, uint32 type, uint32 wrapS, uint32 wrapT, uint32 minFilter, uint32 magFilter)
		{
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
			this->Path = path;

			/* create and enable texture buffer */
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			/* set texture parameters */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);

			/* load texture with stb_image */
			int width, height, n;
			unsigned char * data = stbi_load(Path.c_str(), &width, &height, &n, 0);

			this->Width = width;
			this->Height = height;

			if (!data)
			{
#if DEVELOPMENT == 1
				printf("Error, while loading texture (%s): %s\n", Name.c_str(), stbi_failure_reason());
#endif
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
					printf("Error %d happened while loading Texture 2D \n", err);
				}
#endif

				/* free stbi buffer */
				stbi_image_free(data);

#if DEVELOPMENT == 1
				printf("Loaded Texture2DGL: %s [%d X %d (%d Channels)] #%d \n", Path.c_str(), width, height, n, TextureID);
#endif

				glBindTexture(GL_TEXTURE_2D, 0);
				return;
			}

#if DEVELOPMENT == 1
			printf("Failed to load Texture: %s \n", Path.c_str());
#endif

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
				printf("Error %d happened while initializing Texture2D (%s) \n", err, Name.c_str());
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
			printf("Texture %s destroyed \n", this->Name.c_str());
#endif
		}
	};
};
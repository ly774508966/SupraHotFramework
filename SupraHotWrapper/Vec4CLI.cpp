#include "Vec4CLI.h"
#include "Platform.h"

namespace SupraHot
{
	namespace CLI
	{
		Vec4CLI::Vec4CLI()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		}

		Vec4CLI::Vec4CLI(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		};

		void Vec4CLI::Set(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		void Vec4CLI::Print()
		{
			SHF_PRINTF("[ %f, %f, %f, %f ] \n", x, y, z, w);
		}
	};
};
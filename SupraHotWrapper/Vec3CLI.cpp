#include "Vec3CLI.h"
#include "Platform.h"

namespace SupraHot
{
	namespace CLI
	{
		Vec3CLI::Vec3CLI()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		Vec3CLI::Vec3CLI(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		};

		void Vec3CLI::Set(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void Vec3CLI::Print()
		{
			SHF_PRINTF("[ %f, %f, %f ] \n", x, y, z);
		}
	};
};
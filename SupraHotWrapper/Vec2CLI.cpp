#include "Vec2CLI.h"
#include "Platform.h"

namespace SupraHot
{
	namespace CLI
	{
		Vec2CLI::Vec2CLI()
		{
			this->x = 0.0f;
			this->y = 0.0f;
		}

		Vec2CLI::Vec2CLI(float x, float y)
		{
			this->x = x;
			this->y = y;
		};

		void Vec2CLI::Set(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		void Vec2CLI::Print()
		{
			SHF_PRINTF("[ %f, %f ] \n", x, y);
		}
	};
};
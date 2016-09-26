#include "Vec2.h"

namespace SupraHot
{
	namespace Math
	{
		Vec2::Vec2()
		{
			this->x = 0;
			this->y = 0;
		}

		Vec2::Vec2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		Vec2::~Vec2()
		{
		}
	}
};

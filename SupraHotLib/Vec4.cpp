#include "Vec4.h"

namespace SupraHot
{
	namespace Math
	{
		Vec4::Vec4()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		Vec4::Vec4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vec4::~Vec4()
		{
		}
	};
};
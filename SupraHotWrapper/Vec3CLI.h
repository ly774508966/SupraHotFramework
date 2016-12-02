#pragma once
#include "Wrapper.h"
#include <Vec3.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Vec3CLI
		{
		public:
			float x, y, z;

			Vec3CLI();
			Vec3CLI(float x, float y, float z);
			void Set(float x, float y, float z);
			void Print();
		};
	};
};
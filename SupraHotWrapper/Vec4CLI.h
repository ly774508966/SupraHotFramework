#pragma once
#include "Wrapper.h"
#include <Vec4.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Vec4CLI
		{
		public:
			float x, y, z, w;

			Vec4CLI();
			Vec4CLI(float x, float y, float z, float w);
			void Set(float x, float y, float z, float w);
			void Print();
		};
	};
};
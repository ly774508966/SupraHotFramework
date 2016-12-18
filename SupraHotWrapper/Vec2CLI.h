#pragma once
#include "Wrapper.h"
#include <Vec2.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Vec2CLI
		{
		public:
			float x, y;

			Vec2CLI();
			Vec2CLI(float x, float y);
			void Set(float x, float y);
			void Print();
		};
	};
};
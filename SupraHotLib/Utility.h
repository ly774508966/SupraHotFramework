#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	namespace Utils
	{
		class Utility
		{
			
#ifndef BITSHIFT
		#define BITSHIFT(num) (1 << num)
#endif

		public:
			static std::vector<bool> GetBoolCombinations(uint32 i, uint32 total);
		};
	};
};
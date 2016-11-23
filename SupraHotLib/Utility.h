#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	namespace Utils
	{
		class Utility
		{
		public:
			static std::vector<bool> GetBoolCombinations(uint32 i, uint32 total);
		};
	};
};
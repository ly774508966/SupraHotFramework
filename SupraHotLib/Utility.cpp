#include "Utility.h"
#include <bitset>

namespace SupraHot
{
	namespace Utils
	{

		// NOTE:
		// Remember: The loop should run from i = 0, to i <  pow(2, total)
		std::vector<bool> Utility::GetBoolCombinations(uint32 i, uint32 total)
		{
			std::string binaryString = std::bitset<32>(i).to_string();
			binaryString = binaryString.substr(binaryString.length() - total, total);

			std::vector<bool> booleans;
			for (uint32 j = 0; j < binaryString.size(); j++) {
				booleans.push_back(binaryString.at(j) != '0');
			}

			return booleans;
		}
	};
};
#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	namespace Utils
	{
		class FileReader
		{
		private:
			FileReader();
		public:
			static FileReader* GetInstance();
			~FileReader();

			std::vector<std::string> ReadFile(std::string path);
		};
	};
};

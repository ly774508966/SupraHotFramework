#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	namespace Utils
	{
		class StringUtil
		{
		public:
			// split
			static std::vector<std::string> Split(const std::string& s, const std::string& delim, const bool keep_empty = true);

			// trim
			static std::string &ltrim(std::string &s);
			static std::string &rtrim(std::string &s);
			static std::string &trim(std::string &s);

			static std::string GetFileExtension(const std::string& str);

			static std::string ToLowerCase(const std::string& str);
		};
	};
};
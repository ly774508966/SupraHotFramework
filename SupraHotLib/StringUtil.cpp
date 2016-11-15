#include "StringUtil.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


namespace SupraHot
{
	namespace Utils
	{
		// Autor: Shadow2531
		// Quelle: http://stackoverflow.com/questions/236129/split-a-string-in-c
		std::vector<std::string> StringUtil::Split(const std::string& s, const std::string& delim, const bool keep_empty)
		{
			const std::string str = s + "";

			std::vector<std::string> result;
			if (delim.empty())
			{
				result.push_back(str);
				return result;
			}
			std::string::const_iterator substart = str.begin(), subend;
			while (true)
			{
				subend = search(substart, str.end(), delim.begin(), delim.end());
				std::string temp(substart, subend);
				if (keep_empty || !temp.empty())
				{
					result.push_back(temp);
				}
				if (subend == str.end())
				{
					break;
				}
				substart = subend + delim.size();
			}
			return result;
		}

		// Autor: Evan Teran
		// Quelle: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
		std::string& StringUtil::ltrim(std::string& s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		// Autor: Evan Teran
		// Quelle: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
		std::string& StringUtil::rtrim(std::string& s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		// Autor: Evan Teran
		// Quelle: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
		std::string& StringUtil::trim(std::string& s)
		{
			return ltrim(rtrim(s));
		}

		std::string StringUtil::GetFileExtension(const std::string& str)
		{
			size_t idx = str.rfind(L'.');
			if (idx != std::wstring::npos)
			{
				return str.substr(idx + 1, str.length() - idx - 1);
			}
			else
			{
				return std::string("");
			}
		}

		std::string StringUtil::GetFileNameFromPath(const std::string& str)
		{

			std::string filePath(str);
			size_t idx = filePath.rfind(L'\\');
			if (idx != std::wstring::npos && idx < filePath.length() - 1)
			{
				return filePath.substr(idx + 1);
			}
			else
			{
				idx = filePath.rfind(L'/');
				if (idx != std::wstring::npos && idx < filePath.length() - 1)
					return filePath.substr(idx + 1);
				else
					return filePath;
			}
		}

		std::string StringUtil::GetPathFromFilePath(const std::string& str)
		{
			std::string filename = GetFileNameFromPath(str);
			uint32 idx = str.rfind(filename);
			return str.substr(0, idx);
		}

		std::string StringUtil::ToLowerCase(const std::string& str)
		{
			std::string copy = str;
			std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
			return copy;
		}
	};
};

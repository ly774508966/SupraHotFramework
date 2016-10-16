#include <string>
#include <vector>

static std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
{
	size_t start = 0;
	size_t end = string.find_first_of(delimiters);

	std::vector<std::string> result;

	while (end <= std::string::npos)
	{
		result.emplace_back(string.substr(start, end - start));

		if (end == std::string::npos)
			break;

		start = end + 1;
		end = string.find_first_of(delimiters, start);
	}

	return result;
}

static std::vector<std::string> SplitString(const std::string& string, const char delimiter)
{
	return SplitString(string, std::string(1, delimiter));
}

static std::vector<std::string> SplitStringIntoTokens(const std::string& string)
{
	return SplitString(string, " \t\n");
}

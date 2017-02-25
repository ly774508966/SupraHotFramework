// SupraHotShaderValidator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ShaderValidatorApp.h"

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#include <Controls.h>
#include "stdafx.h"
#include <tchar.h>
#endif

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

using namespace SupraHot;
int _tmain(int argc, char* argv[])
{
	if (argc < 1)
	{
		std::cout << "Usage: Supra Hot ShaderValidator [input_filename](.json)" << std::endl;
		return 0;
	}

	std::string path = argv[1];
	auto split = SplitString(path, "/\\");
	std::string s_Name = split.back();

	SHF_PRINTF("path = %s \n", path.c_str());

	ShaderValidatorApp app;
	app.Init(156, 156, "Shader Validator");
	app.SetFileListener(path);
	app.Loaded();
	app.Run();
	app.Destroy();

	return 0;
}


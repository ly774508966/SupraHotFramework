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


using namespace SupraHot;
int _tmain(int argc, _TCHAR* argv[])
{

	ShaderValidatorApp app;
	app.Init(156, 156, "Shader Validator");
	app.Loaded();
	app.Run();
	app.Destroy();
	return 0;

	// use this as a command line tool

	// add file listener to the first argument we receive

	// open window

	// check file for changes and try to recompile it.
}


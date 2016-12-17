#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#include "stdafx.h"
#include <tchar.h>
#endif

#include "SandBoxApp.h"
namespace SupraHot
{
	namespace Bootstrap
	{
		typedef SandBoxApp MAIN_APP;
		const uint32 WindowWith = 1280;
		const uint32 WindowHeight = 720;
		const std::string WindowTitle = "Supra Hot Sandbox App";
	};
};
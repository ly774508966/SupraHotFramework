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
	};
};
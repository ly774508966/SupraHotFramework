#define DEVELOPMENT 1

#include <string>
#include <cmath>
#include <lua/lua.hpp>

#ifdef PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#ifdef PLATFORM_MAC
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#endif

#ifdef PLATFORM_ANDROID
#include <cstdlib>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>
#endif

// --------
// Macros
// --------

#ifdef PLATFORM_WINDOWS
#define SHF_SPRINTF(dst, sizeinbytes, format) (sprintf_s(dst, sizeinbytes, format))
#define SHF_SSCANF(src, format, ...) (sscanf_s(src, format, ##__VA_ARGS__))
#endif

#ifdef PLATFORM_ANDROID
#define SHF_SPRINTF(dst, sizeinbytes, format) (sprintf(dst, sizeinbytes, format))
#define SHF_SSCANF(src, format, ...) (sscanf(src, format, ##__VA_ARGS__))
#endif

namespace SupraHot
{
	typedef unsigned int uint32;
	typedef int int32;

	typedef unsigned long uint64;
	typedef long int64;

	namespace Graphics {
		class WindowWin32;
		class WindowAndroid;
		class WindowMac;
		class WindowEmscripten;

#ifdef PLATFORM_WINDOWS
		typedef WindowWin32 Window;
#endif

#ifdef PLATFORM_ANDROID
		typedef WindowAndroid Window;
#endif

#ifdef PLATFORM_MAC
		typedef WindowMac Window;
#endif

#ifdef PLATFORM_EMSCRIPTEN
		typedef WindowEmscripten Window;
#endif
	}
};
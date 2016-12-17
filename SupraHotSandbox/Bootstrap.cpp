#include "Bootstrap.h"

#ifdef PLATFORM_WINDOWS
int _tmain(int argc, _TCHAR* argv[])
{
	SupraHot::Bootstrap::MAIN_APP app;
	app.Init(SupraHot::Bootstrap::WindowWith, SupraHot::Bootstrap::WindowHeight, SupraHot::Bootstrap::WindowTitle);
	app.Loaded();
	app.Run();
	app.Destroy();
	return 0;
}
#endif

#ifdef PLATFORM_EMSCRIPTEN
int main()
{
	SupraHot::Bootstrap::MAIN_APP app;
	app.Init(SupraHot::Bootstrap::WindowWith, SupraHot::Bootstrap::WindowHeight, SupraHot::Bootstrap::WindowTitle);
	app.Loaded();
	app.Run();
	app.Destroy();
	return 0;
}
#endif
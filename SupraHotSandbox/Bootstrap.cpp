#include "Bootstrap.h"

#ifdef PLATFORM_WINDOWS
int _tmain(int argc, _TCHAR* argv[])
{
	SupraHot::Bootstrap::MAIN_APP app;
	app.Init(640, 480, "Demo Application");
	app.Run();
	app.Destroy();
	return 0;
}
#endif

#ifdef PLATFORM_EMSCRIPTEN
int main()
{
	SupraHot::Bootstrap::MAIN_APP app;
	app.Init(640, 480, "Demo Application");
	app.Run();
	app.Destroy();
	return 0;
}
#endif
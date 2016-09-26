#include "LuaVM.h"

namespace SupraHot
{
	namespace Scripting
	{
		LuaVM::LuaVM()
		{
			state = luaL_newstate();
			luaL_openlibs(state);
		}

		LuaVM::~LuaVM()
		{
			lua_close(state);
		}

		LuaVM* instance = nullptr;
		LuaVM* LuaVM::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = new LuaVM();
			}
			return instance;
		}

		bool LuaVM::RunFile(const char* path)
		{
			return luaL_dofile(state, path);
		}

		bool LuaVM::CallFunction(const char* functionName, const char* signature, ...)
		{
			va_list vl;
			int narg, nres;  //number of arguments and results 
			va_start(vl, signature);
			lua_getglobal(state, functionName);  // get function 

#if LUA_IS_FUNCTION_CHECKING == 1
			if (lua_isfunction(state, lua_gettop(state)))
			{
#endif
				// push arguments 
				narg = 0;
				while (*signature)
				{
					switch (*signature++)
					{
					case 'd':  // double argument
						lua_pushnumber(state, va_arg(vl, double));
						break;

					case 'i':  // int argument
						lua_pushnumber(state, va_arg(vl, int));
						break;

					case 's':  // string argument
						lua_pushstring(state, va_arg(vl, char *));
						break;

					case '>':
						goto endwhile;

					default:
						Error(state, "invalid option (%c)", *(signature - 1));
					}
					narg++;
					luaL_checkstack(state, 1, "too many arguments");
				} endwhile:

				// do the call
				nres = (int)strlen(signature);  // number of expected results
				if (lua_pcall(state, narg, nres, 0) != 0)  // do the call
				{
					Error(state, "error running function `%s': %s",
						functionName, lua_tostring(state, -1));
				}

				// retrieve results
				nres = -nres;  // stack index of first result
				while (*signature)
				{  // get results
					switch (*signature++)
					{
					case 'd':  // double result 
						if (!lua_isnumber(state, nres))
							Error(state, "wrong result type");
						*va_arg(vl, double *) = lua_tonumber(state, nres);
						break;

					case 'i':  // int result
						if (!lua_isnumber(state, nres))
							Error(state, "wrong result type");
						*va_arg(vl, int *) = (int)(lua_tonumber(state, nres));
						break;

					case 's':  // string result 
						if (!lua_isstring(state, nres))
							Error(state, "wrong result type");
						*va_arg(vl, const char **) = lua_tostring(state, nres);
						break;

					default:
						Error(state, "invalid option (%c)", *(signature - 1));
					}
					nres++;
				}
#if LUA_IS_FUNCTION_CHECKING == 1
			}
#endif
			va_end(vl);
			return true;
		}

		void LuaVM::CloseLuaVM()
		{
#if _DEBUG
			printf("CloseLuaVM() \n");
#endif
			delete instance;
		}

		void LuaVM::Error(lua_State* L, const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);

	//The following code should always
	//execute the exit statement.
	//
	//It is written like this to suppress 
	//'noreturn' warnings.
	int a = 0;
	int b = 0;
	if (!(a != b && b == 0 && a == 0))
	{
		exit(EXIT_FAILURE);
	}
}
	};
};
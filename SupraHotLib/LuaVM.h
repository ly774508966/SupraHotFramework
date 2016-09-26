#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Scripting
	{
		#define LUA_RUN_START_FILE 1
		#define LUA_INIT 1
		#define LUA_UPDATE 1
		#define LUA_RENDER 1
		#define LUA_DEBUG 1
		#define LUA_IS_FUNCTION_CHECKING 0

		class LuaVM
		{
		private:
			lua_State* state;
			LuaVM();
		public:
			static LuaVM* GetInstance();
			bool RunFile(const char* path);
			bool CallFunction(const char *functionName, const char *signature, ...);
			void CloseLuaVM();
			static void Error(lua_State* L, const char* fmt, ...);

			~LuaVM();
		};

	};
};

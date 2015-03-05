
#ifndef _IRIS_SCRIPT_
#define _IRIS_SCRIPT_

#include <string>

extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Script
			{
			
				private:
				
				lua_State * L;
				
				static Script * instance;
				
				Script(std::string filename);
				~Script();
				
				public:
				
				
				static Script * Get();
				static void Init(std::string filename);
				static void Run();
				static void Quit();
			};
		}
	}
}

#endif

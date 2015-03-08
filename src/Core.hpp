
#ifndef _IRIS_CORE_
#define _IRIS_CORE_

#include "Scene.hpp"

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
			class Core
			{
			
				private:
				
				lua_State * L;
				
				static Core * instance;
				
				Core(std::string dirname);
				~Core();
				
				public:
				
				Scene scene;
				std::string basedir;
				
				
				void Run();
				
				static Core * Get();
				static Core * Init(std::string dirname);
				
				static void Quit();
			};
		}
	}
}

#endif

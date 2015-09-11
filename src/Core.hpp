
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
				
				Core();
				
				
				public:
				
				std::string basedir;
				Scene scene;
				
				virtual ~Core();
				
				static Core * Get();
				
				void OnProgress(float p);
				void Compile(std::string path);
				void Run();
				
			};
		}
	}
}

#endif

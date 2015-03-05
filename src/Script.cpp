

#include "Script.hpp"

#include <exception>

using namespace std;
using namespace com::toxiclabs::iris;

Script * Script::instance = nullptr;

/*!
	Set screen settings
	params:
	int width
	int height
	int samples per pixel [optional]
	string output bitmap filename [optional]
*/
int SetScreen(lua_State * L)
{
	if(lua_gettop(L)>1)
	{
		int width = lua_tointeger(L,1);
		int height = lua_tointeger(L,2);
		int samples=1;
		string out_filename="out.png";
		
		if(lua_gettop(L)>2)
		{
			samples=lua_tointeger(L,3);
			
			if(lua_gettop(L)>3)
			{
				out_filename = lua_tostring(L,4);
			}
		}
	}
	
	return 0;
}

/*!
	Loads a wavefront (obj) mesh
	params:
	string filename
*/
int LoadMesh(lua_State * L)
{
	if(lua_gettop(L)>0)
	{
		const char * filename = lua_tostring(L,1);
	}
	
	return 0;
}



Script::Script(string filename)
{
	L = lua_open();
	luaL_openlibs(L);
	
	
	//register functions
	lua_register(L,"SetScreen",SetScreen);
	lua_register(L,"LoadMesh",LoadMesh);
	
	int status = luaL_loadfile(L, filename.c_str());
	
	if(status!=0)
		throw runtime_error("Failed to read script");
		

	lua_pcall(L,0,LUA_MULTRET,0);
}


Script::~Script()
{
}


Script * Script::Get()
{
	return Script::instance;
}


void Script::Init(string filename)
{
	Script::instance = new Script(filename);
}

void Script::Run()
{

}

void Script::Quit()
{
	delete Script::instance;
}

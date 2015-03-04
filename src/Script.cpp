

#include "Script.hpp"

extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

using namespace std;
using namespace com::toxiclabs::iris;

Script * Script::instance = nullptr;

Script::Script(string filename)
{
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

void Script::Quit()
{
	delete Script::instance;
}

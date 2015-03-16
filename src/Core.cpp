

#include "Core.hpp"
#include "Scene.hpp"
#include "MeshLoader.hpp"
#include "PathTracer.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace com::toxiclabs::iris;

Core * Core::instance = nullptr;

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
		
		Core * core = Core::Get();
		MeshLoader::Load(core->basedir+"/"+filename,core->scene.triangles,core->scene.materials);
	}
	
	return 0;
}

/*!
	Sets camera settings
	params:
	vector origin
	vector target
*/
int SetCamera(lua_State * L)
{
	if(lua_gettop(L)>1)
	{
		Vector position;
		Vector target;
		
		lua_rawgeti(L,1,1);
		lua_rawgeti(L,1,2);
		lua_rawgeti(L,1,3);
		position.z=lua_tonumber(L,-1);
		position.y=lua_tonumber(L,-2);
		position.x=lua_tonumber(L,-3);
		position.w=1.0f;
		
		lua_rawgeti(L,2,1);
		lua_rawgeti(L,2,2);
		lua_rawgeti(L,2,3);
		target.z=lua_tonumber(L,-1);
		target.y=lua_tonumber(L,-2);
		target.x=lua_tonumber(L,-3);
		target.w=1.0f;
		
		Core * core = Core::Get();
		core->scene.SetCamera(new Camera(position,target));
	}
	
	return 0;
}

int AddMaterial(lua_State * L)
{
	if(lua_gettop(L)>1)
	{
		const char * name = lua_tostring(L,1);
		const char * filename = lua_tostring(L,2);
		
		Core * core = Core::Get();
		
		Material * mat = new Material();
		
		mat->name=name;
		mat->Kd=Spectrum(filename);
		
		core->scene.materials.push_back(mat);
		
		cout<<"Added material: "<<name<<endl;
		cout<<"\t"<<"Kd:"<<filename<<endl;
	}
	
	return 0;
}

Core::Core(string dirname)
{

	basedir=dirname;
	
	L = luaL_newstate();
	luaL_openlibs(L);
	
	
	//register functions
	lua_register(L,"SetScreen",SetScreen);
	lua_register(L,"LoadMesh",LoadMesh);
	lua_register(L,"SetCamera",SetCamera);
	lua_register(L,"AddMaterial",AddMaterial);
	
}


Core::~Core()
{
}


Core * Core::Get()
{
	return Core::instance;
}


Core * Core::Init(string dirname)
{
	Core::instance = new Core(dirname);
	
	return Core::instance;
}

void Core::Run()
{

	//script execution
	string filename = basedir +"/setup.lua";
	int status = luaL_loadfile(L, filename.c_str());
	
	if(status!=0)
		throw runtime_error("Failed to read Core");
		

	lua_pcall(L,0,LUA_MULTRET,0);
	
	
	//checking scene settings before rendering
	
	scene.ApplyCamera();
	
	//raytracing
	PathTracer tracer(scene);
	
	tracer.Run();
	
	
}

void Core::Quit()
{
	delete Core::instance;
}

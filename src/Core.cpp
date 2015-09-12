

#include "Core.hpp"
#include "Scene.hpp"
#include "MeshLoader.hpp"
#include "PathTracer.hpp"

#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;
using namespace std::placeholders;
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
int SetImage(lua_State * L)
{

	Core * core = Core::Get();
	
	
	if(lua_gettop(L)>1)
	{
		int width = lua_tointeger(L,1);
		int height = lua_tointeger(L,2);

		core->scene.params["image.width"]=width;
		core->scene.params["image.height"]=height;
		
		int samples;
		string out_filename;
		
		if(lua_gettop(L)>2)
		{
			samples=lua_tointeger(L,3);
			
			core->scene.params["image.subsamples"]=samples;
			
			if(lua_gettop(L)>3)
			{
				out_filename = lua_tostring(L,4);
				core->scene.params["image.output_name"]=out_filename;
			}
		}
	}
	

	
	
	
	return 0;
}

int SetCPU(lua_State * L)
{

	Core * core = Core::Get();
	
	if(lua_gettop(L)>0)
	{
		int nthreads = lua_tointeger(L,1);
		core->scene.params["system.threads"]=nthreads;
		
	}
	return 0;
}

/*!
	Number of samples used in integration
*/
int SetPathTracer(lua_State * L)
{
	Core * core = Core::Get();
	
	if(lua_gettop(L)>0)
	{
		int psamples = lua_tointeger(L,1);
		core->scene.params["pathtracer.samples"]=psamples;
		
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

Core::Core()
{

	
	L = luaL_newstate();
	luaL_openlibs(L);
	
	
	//register functions
	
	lua_register(L,"SetScreen",SetImage);//deprecated
	lua_register(L,"SetImage",SetImage);
	lua_register(L,"SetCPU",SetCPU);
	lua_register(L,"SetPathTracer",SetPathTracer);
	lua_register(L,"LoadMesh",LoadMesh);
	lua_register(L,"SetCamera",SetCamera);
	lua_register(L,"AddMaterial",AddMaterial);
	
}


Core::~Core()
{
	Core::instance=nullptr;
}


Core * Core::Get()
{
	if(Core::instance==nullptr)
		Core::instance=new Core();
	
	return Core::instance;
}

void Core::OnProgress(float p)
{
	//cout<<p<<"%"<<endl;
	
	cout<<'\r';
	cout<<"Rendering:[";
	
	int t = p*40;
	
	
	for(int n=0;n<t;n++)
	{
		cout<<"*";
	}
	for(int n=t;n<40;n++)
	{
		cout<<" ";
	}
	
	cout<<"] "<<fixed<<setprecision(2)<<p<<"%"<<flush;
	
}

void Core::Compile(string path)
{
	basedir=path;
	string filename = basedir +"/setup.lua";
	int status = luaL_loadfile(L, filename.c_str());
	
	if(status!=0)
		throw runtime_error("Failed to read:"+filename);
		

	lua_pcall(L,0,LUA_MULTRET,0);
	
	scene.ApplyCamera();
}

void Core::Run()
{

	auto start = std::chrono::high_resolution_clock::now();
	
	//raytracing
	PathTracer tracer(scene);
	
	tracer.Run(std::bind(&Core::OnProgress,this,_1));
	
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout<<endl;
	cout<<"Render time: "<<elapsed.count()<<"ms"<<endl;
	
}



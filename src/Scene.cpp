
#include "Scene.hpp"

using namespace com::toxiclabs::iris;


Scene::Scene()
{
	camera=nullptr;
	
	//default settings
	
	params["image.width"]=800;
	params["image.height"]=600;
	params["image.subsamples"]=2;
	params["image.output_name"]="out.png";
	
	params["system.threads"]=2;
	
	params["pathtracer.samples"]=16;
	
	//hardcoded plane
	geometries.push_back(new Plane(0.0f));
	
	//hardcoded atmosphere
	atmosphere = new Atmosphere();
	
}

Scene::~Scene()
{
	delete atmosphere;
	delete camera;
}

void Scene::SetCamera(Camera * camera)
{
	this->camera=camera;
}

void Scene::SetAtmosphere(Atmosphere * atmosphere)
{
	this->atmosphere=atmosphere;
}


void Scene::ApplyCamera()
{

	if(camera==nullptr)
	{
		Vector origin(2.0,8.0,-20.0);
		Vector target(0.0,2.0,0.0);
		camera = new Camera(origin,target);
	}

	
	Matrix & mC = camera->matrix;
	
	for(Geometry * geometry : geometries)
	{
		geometry->Mult(mC);
	}
	
	atmosphere->Mult(mC);
}

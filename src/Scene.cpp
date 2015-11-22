
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
	
}

void Scene::SetCamera(Camera * camera)
{
	if(this->camera!=nullptr)
	{
		delete this->camera;
	}
	
	this->camera=camera;
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
	
	
}

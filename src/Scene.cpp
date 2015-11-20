
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
	
	for(Triangle * triangle : triangles)
	{
		triangle->vertices[0]=triangle->vertices[0] * mC;
		triangle->vertices[1]=triangle->vertices[1] * mC;
		triangle->vertices[2]=triangle->vertices[2] * mC;
		
		triangle->vertices[0].Homogeneus();
		triangle->vertices[1].Homogeneus();
		triangle->vertices[2].Homogeneus();
		
		triangle->normals[0]=triangle->normals[0] * mC;
		triangle->normals[1]=triangle->normals[1] * mC;
		triangle->normals[2]=triangle->normals[2] * mC;
		
		triangle->pnormal=triangle->pnormal * mC;
		
		triangle->D=triangle->vertices[0] * triangle->pnormal;
	}
	
}


#include "Scene.hpp"

using namespace com::toxiclabs::iris;


void Scene::ApplyCamera(int n)
{
	Camera * camera = cameras[n];
	
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
	}
}

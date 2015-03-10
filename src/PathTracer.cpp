
#include "PathTracer.hpp"
#include "MeshLoader.hpp"
#include "Spectrum.hpp"
#include "KdTree.hpp"


#include <iostream>
#include <chrono>
#include <cmath>

bool first=true;
bool first2=true;

using namespace std;
using namespace com::toxiclabs::iris;


Spectrum sunlight("d65.spd");
Spectrum material("macbeth-5.spd");

Vector sun(0.5,0.5,-0.5,0.0);



PathTracer::PathTracer(Scene & scene)
{
	this->scene=scene;
	
	sun.Normalize();
	
	
	/* default render settings */
	
	width=800;
	height=600;
	num_threads=2;
	samples=2;
	
	/* creating render target */
	image = new fipImage(FIT_BITMAP,width,height,32);
	
	
	
	/* computing chunks */
	int wchunk=60;
	int hchunk=60;
		
	while(width%wchunk!=0)
	{
		wchunk++;
	}
	
	while(height%hchunk!=0)
	{
		hchunk++;
	}
	
	for(int i=0;i<(width/wchunk);i++)
	{
		for(int j=0;j<(height/hchunk);j++)
		{
			
			RenderChunk * chunk = new RenderChunk();
			
			chunk->x=i*wchunk;
			chunk->y=j*hchunk;
			chunk->w=wchunk;
			chunk->h=hchunk;
			
			chunks.push_back(chunk);
			
		}
	}
	
	cout<<"Chunk size: "<<wchunk<<"x"<<hchunk<<endl;
	
	
	
	
	tree = new KdTree(scene.triangles);
	
			
}

PathTracer::~PathTracer()
{
	cout<<"[PathTracer] closing..."<<endl;
	
	
	
	delete tree;
	delete image;
}

void PathTracer::Run()
{
	
	auto start = std::chrono::high_resolution_clock::now();
		
	cout<<"[PathTracer] run"<<endl;
	
	cout<<"[PathTracer] Spawning threads"<<endl;
	
	vector<thread> threads;
	
	for(int n=0;n<num_threads;n++)
	{
		threads.push_back(thread(&PathTracer::RenderThread,this,n));
	}
	
	for(int n=0;n<threads.size();n++)
	{
		threads[n].join();
	}
	
	
		
	image->save("out.png");
	
	cout<<"[PathTracer] render finished"<<endl;
	
	auto end = std::chrono::high_resolution_clock::now();
	
	auto elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	
	cout<<"Sunlight:"<<sunlight.ToString()<<endl;
	
	cout<<"Render time: "<<elapsed.count()<<"ms"<<endl;
}

RenderChunk * PathTracer::GetChunk()
{
	RenderChunk * chunk = nullptr;
	
	chunk_mutex.lock();
	
	if(chunks.size()>0)
	{
		chunk = chunks.back();
		chunks.pop_back();
	}
	
	chunk_mutex.unlock();
	
	return chunk;
}

void PathTracer::CommitChunk(RenderChunk * chunk)
{
	chunk_mutex.lock();
	
	cout<<"[PathTracer] Chunk "<<chunk->x<<","<<chunk->y<<" commited"<<endl;
	
	for(int j=chunk->y;j<(chunk->y+chunk->h);j++)
	{
		for(int i=chunk->x;i<(chunk->x+chunk->w);i++)
		{
			int px = i-chunk->x;
			int py = j-chunk->y;
			
			RGBQUAD pixel;
			ColorRGB rgb;
			rgb = chunk->image[px+py*chunk->w].ToRGB();
			
						
			pixel.rgbRed=255*rgb.r;
			pixel.rgbGreen=255*rgb.g;
			pixel.rgbBlue=255*rgb.b; 
			pixel.rgbReserved=255;
			
			image->setPixelColor(i,(height-1)-j,&pixel);
		}
	}
	
	delete [] chunk->image;
	
	chunk_mutex.unlock();
}

void PathTracer::RenderThread(int id)
{

	cout<<"[PathTracer] Thread "<<id<<" entered rendering"<<endl;
	
	

	float fov=45.0f;
	float beta=fov/2.0f;
	float Z=6.0f; //Hack camera Z coord
	float v = Z * tan(DegToRad(beta));
	float fwidth,fheight;
	float pw,ph; //pixel dimensions
	float pdw,pdh; //pixel fraction dimension
	
	//far plane dimensions
	fheight = v * 2.0f;
	fwidth = fheight * 1.333f; // hardcoded aspect ratio
	 
	//pixel dimensions
	pw = fwidth/width;
	ph = fheight/height;
	
	pdw = pw/samples;
	pdh = ph/samples;
	
	Vector origin;
	Vector direction;

	Spectrum incoming;
	Spectrum outcoming;
	


	RenderChunk * chunk = GetChunk();
	while(chunk!=nullptr)
	{
		chunk->image=new ColorXYZ[chunk->w * chunk->h];
	
		for(int j=chunk->y;j<(chunk->y+chunk->h);j++)
		{
			for(int i=chunk->x;i<(chunk->x+chunk->w);i++)
			{
			
				int px = i-chunk->x;
				int py = j-chunk->y;
				
				origin.Set(0.0f,0.0f,-Z,1.0f);
				
				outcoming.Clear();
				
				for(int sy=0;sy<samples;sy++)
				{
					for(int sx=0;sx<samples;sx++)
					{
						direction.Set
						((i*pw)+((pdw*sx)+pw*0.25f)-(width*pw*0.5f),
						((pdh*sy)+ph*0.25f)+(height*ph*0.5f)-(j*ph),5.0f,
						0.0f);
	
						direction = direction - origin;
				
						direction.Normalize();
						//RayCast(id,origin,direction,incoming);
						incoming=Ray(RayType::Visibility,origin,direction);
												
						incoming=incoming*(1.0f/(samples*samples));
						outcoming=outcoming+incoming;
					}
				}
				
				
				chunk->image[px+py*chunk->w]=outcoming.ToXYZ();
			}
		}
	
		/* render goes here */
		
		CommitChunk(chunk);
		chunk = GetChunk();
	}

	cout<<"[PathTracer] Thread "<<id<<" exit rendering"<<endl;
}

void PathTracer::RayCast(int id,Vector & origin,Vector & direction,Spectrum & output)
{
	float min_dist=100000.0f;
	Vector collision;
	Vector oc;
	float dist;
	double r[4];
	
	Vector target_collision;
	Triangle * target_triangle=nullptr;
	
	output.Clear();
	

	vector<KdNode *> nodes = tree->Traverse(origin,direction);
	
	for(KdNode * node : nodes)
	{
		vector<Triangle *>::iterator q;
				
		if(!node->RayCollision(origin,direction))
			continue;
		
		for(q=node->triangles.begin();q!=node->triangles.end();q++)
		{
			Triangle * triangle = *q;
			if(triangle->RayCollision(origin,direction,collision))
			{
				oc=collision-origin;
				dist=oc.Module();
			
				if(dist<min_dist)
				{
					min_dist=dist;
					target_collision=collision;
					target_triangle=triangle;
				
				}
			}
		}
	}
	
	
	
	
	if(target_triangle!=nullptr)
	{
		
		Vector perturbated_normal;
						
		Spectrum incoming;
		
		incoming.Clear();
		
		
		
		
		int samples=4;
		
		for(int n=0;n<samples;n++)
		{
			for(int m=0;m<samples;m++)
			{
				float r0=n/(float)samples;
				float r1=(n+1)/(float)samples;

				float r2=m/(float)samples;
				float r3=(m+1)/(float)samples;
				
				float ra = (float)rand()/RAND_MAX;
				float rb = (float)rand()/RAND_MAX;
				
				if(first)
				{
					cout<<r0<<" "<<r1<<" "<<r2<<" "<<r3<<" "<<ra<<" "<<rb;
				}
				
				r0 = r0+ ((r1-r0)*ra);
				r2 = r2+ ((r3-r2)*rb);
				
				if(first)
				{
					cout<<" "<<r0<<" "<<r2<<endl;
				}	
					
				perturbated_normal=target_triangle->PerturbateNormal(0.98f,r0,r2);
				
				if(first)
				{
					target_triangle->normals[0].Print();
					perturbated_normal.Print();
				}
				
				
						
				float cosPhi = perturbated_normal * target_triangle->normals[0];
				
				Spectrum I = PathTrace(target_collision,perturbated_normal,target_triangle,1);
				I=I*(cosPhi/3.1416f);
				
				
				incoming = incoming + I;
			}
		}
		
		if(first)
		{
			first=false;
		}
	
		output = incoming * (1.0f/(samples*samples));
		output = output * material;
		
		/*
		Vector w = target_collision - light;
		w.Normalize();
		w.Negate();
		
		float cosPhi = w * target_triangle->normals[0];
		if(cosPhi<0.0f)cosPhi=0.0f;
		if(cosPhi>1.0f)cosPhi=1.0f;
		output =material *  cosPhi;
		output = output + incoming;
		
		*/
	}
}

Spectrum PathTracer::PathTrace(Vector & origin, Vector & direction,Triangle * source,int depth)
{
	Spectrum energy;
	float min_dist=100000.0f;
	Vector collision;
	Vector oc;
	float dist;
	
	Vector target_collision;
	Triangle * target_triangle=nullptr;
	
	energy.Clear();
	
	
	vector<KdNode *> nodes = tree->Traverse(origin,direction);
	
	for(KdNode * node : nodes)
	{
		vector<Triangle *>::iterator q;
				
		if(!node->RayCollision(origin,direction))
			continue;
		
		for(q=node->triangles.begin();q!=node->triangles.end();q++)
		{
			Triangle * triangle = *q;
			
			if(triangle==source)
				continue;
				
			if(triangle->RayCollision(origin,direction,collision))
			{
				oc=collision-origin;
				dist=oc.Module();
			
				if(dist<min_dist)
				{
					min_dist=dist;
					target_collision=collision;
					target_triangle=triangle;
				
				}
			}
		}
	}
		
	
	if(target_triangle!=nullptr)
	{
		/* global illumination */
		//energy.data[16]=10.0f;
		
	}
	else
	{
		/* ambient illumination */
		//energy.data[1]=30.0f;
		//energy.data[2]=30.0f;
		float wsun=sun * direction;
		energy=sunlight * (0.15*wsun);
	}	
	
	return energy;
}


Spectrum PathTracer::Ray(RayType type,Vector & origin,Vector & direction,
Triangle * source,int depth)
{

	Spectrum energy;
	Spectrum diffuse;
	Spectrum specular;
	Spectrum incoming;

	float min_dist=100000.0f;
	Vector collision;
	Vector oc;
	float dist;
	
	float r0,r1,r2,r3,ra,rb;
	Vector perturbated_normal;
	
	int samples=4;//this should be parametrized elsewhere
	
	Vector target_collision;
	Triangle * target_triangle=nullptr;
	
	
	vector<KdNode *> nodes = tree->Traverse(origin,direction);
	
	for(KdNode * node : nodes)
	{
		vector<Triangle *>::iterator q;
				
		if(!node->RayCollision(origin,direction))
			continue;
		
		for(q=node->triangles.begin();q!=node->triangles.end();q++)
		{
			Triangle * triangle = *q;
			
			if(triangle==source)
				continue;
			
			if(triangle->RayCollision(origin,direction,collision))
			{
				oc=collision-origin;
				dist=oc.Module();
			
				if(dist<min_dist)
				{
					min_dist=dist;
					target_collision=collision;
					target_triangle=triangle;
				
				}
			}
		}
	}
	
	
	if(target_triangle!=nullptr)
	{
		switch(type)
		{
			case RayType::Visibility:
			
			//gathering diffuse light
			diffuse.Clear();
			
			for(int n=0;n<samples;n++)
			{
				for(int m=0;m<samples;m++)
				{
					r0=n/(float)samples;
					r1=(n+1)/(float)samples;

					r2=m/(float)samples;
					r3=(m+1)/(float)samples;
				
					ra = (float)rand()/RAND_MAX;
					rb = (float)rand()/RAND_MAX;
				
				
					r0 = r0+ ((r1-r0)*ra);
					r2 = r2+ ((r3-r2)*rb);
								
					
					perturbated_normal=target_triangle->PerturbateNormal(0.98f,r0,r2);
					incoming=Ray(RayType::Diffuse,target_collision,perturbated_normal,target_triangle,depth + 1);
					diffuse=diffuse+incoming;
				}
			}
			
			diffuse=diffuse*(1.0f/(samples*samples));
			energy=diffuse*material;
			break;
			
		}
	}
	else
	{
		/*
		ToDo:
		Get proper energy from sky/sunlight
		*/
		
		energy=sunlight*0.15;
		
		
	}
	
	return energy;
	
}

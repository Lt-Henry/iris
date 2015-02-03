
#include "Core.hpp"
#include "MeshLoader.hpp"
#include "Spectrum.hpp"
#include "KdTree.hpp"


#include <iostream>
#include <chrono>
#include <cmath>




using namespace std;
using namespace com::toxiclabs::iris;


Spectrum sunlight("fluorescent.spd");
Spectrum material("macbeth-5.spd");

Vector light(2.0,8.0,0.0,1.0);



Core::Core(int argc,char * argv[])
{
	if(argc<2)
	{
		cout<<"Not enough parameters"<<endl;
	}
	else
	{
		cout<<"[Core] loading:"<<argv[1]<<endl;
		MeshLoader::Load(argv[1],scene.triangles,scene.materials);
	}
	
	
	
	
	/* default render settings */
	
	width=800;
	height=600;
	num_threads=2;
	samples=1;
	
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
	
	/* create a default camera */
	Vector origin(2.0,8.0,-20.0);
	Vector target(0.0,2.0,0.0);
	Camera * camera = new Camera("camera-0",origin,target);
	scene.cameras.push_back(camera);
	
	scene.ApplyCamera();
	
	
	tree = new KdTree(scene.triangles);
	
			
}

Core::~Core()
{
	cout<<"[Core] closing..."<<endl;
	
	for(int n=0;n<num_threads;n++)
		gsl_qrng_free(qr[n]);
	
	delete tree;
	delete image;
}

void Core::Run()
{
	
	auto start = std::chrono::high_resolution_clock::now();
		
	cout<<"[Core] run"<<endl;
	
	cout<<"[Core] Spawning threads"<<endl;
	
	vector<thread> threads;
	
	for(int n=0;n<num_threads;n++)
	{
		threads.push_back(thread(&Core::RenderThread,this,n));
	}
	
	for(int n=0;n<threads.size();n++)
	{
		threads[n].join();
	}
	
	
		
	image->save("out.png");
	
	cout<<"[Core] render finished"<<endl;
	
	auto end = std::chrono::high_resolution_clock::now();
	
	auto elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	
	cout<<"Render time: "<<elapsed.count()<<"ms"<<endl;
}

RenderChunk * Core::GetChunk()
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

void Core::CommitChunk(RenderChunk * chunk)
{
	chunk_mutex.lock();
	
	cout<<"[Core] Chunk "<<chunk->x<<","<<chunk->y<<" commited"<<endl;
	
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

void Core::RenderThread(int id)
{

	cout<<"[Core] Thread "<<id<<" entered rendering"<<endl;
	
	/* quasi-random generator */
	qr[id]=gsl_qrng_alloc (gsl_qrng_sobol, 2);

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
						RayCast(id,origin,direction,incoming);
												
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

	cout<<"[Core] Thread "<<id<<" exit rendering"<<endl;
}

void Core::RayCast(int id,Vector & origin,Vector & direction,Spectrum & output)
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
		
		//gsl_qrng_init(qr[id]);
		
		
		int samples=6;
		
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
				
				r0 = r0+ ((r1-r0)*ra);
				r2 = r2+ ((r3-r2)*rb);
				perturbated_normal=target_triangle->PerturbateNormal(0.98f,r0,r2);
						
				float cosPhi = perturbated_normal * target_triangle->normals[0];
				
				Spectrum I = PathTrace(target_collision,perturbated_normal,target_triangle,1);
				I=I*cosPhi;
				
				
				incoming = incoming + I;
			}
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

Spectrum Core::PathTrace(Vector & origin, Vector & direction,Triangle * source,int depth)
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
		energy=sunlight * 0.1f;
	}	
	
	return energy;
}

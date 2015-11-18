
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





PathTracer::PathTracer(Scene & scene)
{
	this->scene=scene;
	
	
	
	
	
	width=scene.params["image.width"].GetInt();
	height=scene.params["image.height"].GetInt();

	samples=scene.params["image.subsamples"].GetInt();
	
	num_threads=scene.params["system.threads"].GetInt();
	
	psamples=(int)std::sqrt(scene.params["pathtracer.samples"].GetInt());
	
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
	
	int nwchunks=width/wchunk;
	int nhchunks=height/hchunk;
	
	total_chunks=nwchunks*nhchunks;
	
	for(int i=0;i<nwchunks;i++)
	{
		for(int j=0;j<nhchunks;j++)
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
	
	
	
	
	//cout<<"sunlight: "<<sunlight.ToString()<<endl;
	
	tree = new KdTree(scene.triangles);
	
	Material * white = new Material();
	vector<float> Kd(32,0.3f);
	white->Kd=Kd;
	this->scene.materials.push_back(white);
	
	
	//hack ambient data
	sun_pos=Vector(0.5,0.5,-0.5,0.0);
	sun_pos.Normalize();
	
	/* 
	 Sun spectrum from:
	 http://www.pveducation.org/pvcdrom/appendices/standard-solar-spectra
	 
	 AM1.5 Global ASTMG173
	*/
	vector<float> e={0.79699,1.1141,1.0485,1.1232 ,0.87462,1.3499,1.5595,1.5291,
	 1.5077,1.6181,1.6224,1.5451 ,1.5481,1.5236,1.5446,1.4825 ,1.5399,1.474,1.4816,1.502 ,
	1.3709,1.4753,1.4686,1.4739 ,1.3924,1.434,1.3594,1.3992 ,1.4196,1.3969,1.1821,1.2823};
	//sun_energy=Spectrum(e);
	
	sun_energy=Spectrum(9000);
	sun_energy.Normalize();
	
}

PathTracer::~PathTracer()
{
	
	delete tree;
	delete image;
}

void PathTracer::Run(function<void(float)> progress)
{
	this->progress=progress;
	
	
		
	
	vector<thread> threads;
	
	for(int n=0;n<num_threads;n++)
	{
		threads.push_back(thread(&PathTracer::RenderThread,this,n));
	}
	
	for(int n=0;n<threads.size();n++)
	{
		threads[n].join();
	}
	
	
	//maybe this is not the right place
	image->save(scene.params["image.output_name"].GetString().c_str());
	
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
	
	//cout<<"[PathTracer] Chunk "<<chunk->x<<","<<chunk->y<<" commited"<<endl;
	
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
	
	float p=1.0f - (chunks.size()/(float)total_chunks);
	progress(p);
	
	chunk_mutex.unlock();
}

void PathTracer::RenderThread(int id)
{

	//cout<<"[PathTracer] Thread "<<id<<" entered rendering"<<endl;
	
	

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
	Vector intersection;

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
						intersection.Set
						((i*pw)+((pdw*sx)+pw*0.25f)-(width*pw*0.5f),
						((pdh*sy)+ph*0.25f)+(height*ph*0.5f)-(j*ph),5.0f,
						0.0f);
	
						direction = intersection - origin;
				
						direction.Normalize();
						//RayCast(id,origin,direction,incoming);
						incoming=Ray(RayType::Visibility,intersection,direction);
												
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

	//cout<<"[PathTracer] Thread "<<id<<" exit rendering"<<endl;
}



Spectrum PathTracer::Ray(RayType type,Vector & origin,Vector & direction,
Triangle * source,int depth)
{

	Material * material;
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
	Vector di;
	float f;
		
	
	Vector target_collision;
	Triangle * target_triangle=nullptr;
	
	if(depth>3)//hardcoded!
	{
		return energy;
	}
	
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
	
		Vector normal = target_triangle->GetAveragedNormal(target_collision);
		//material=scene.materials[target_triangle->material];
		material=scene.materials[0];
		
		
		switch(type)
		{
		
			case RayType::Specular:
			case RayType::Visibility:
			
			//gathering diffuse light
			diffuse.Clear();
			
			for(int n=0;n<psamples;n++)
			{
				for(int m=0;m<psamples;m++)
				{
					r0=n/(float)psamples;
					r1=(n+1)/(float)psamples;

					r2=m/(float)psamples;
					r3=(m+1)/(float)psamples;
				
					ra = (float)rand()/RAND_MAX;
					rb = (float)rand()/RAND_MAX;
				
				
					r0 = r0+ ((r1-r0)*ra);
					r2 = r2+ ((r3-r2)*rb);
								
					
					perturbated_normal=target_triangle->PerturbateNormal(0.98f,r0,r2);
					incoming=Ray(RayType::Diffuse,target_collision,perturbated_normal,target_triangle,depth + 1);
					//incoming=incoming*INV_PI;
					diffuse=diffuse+incoming;
				}
			}
			
			diffuse=diffuse*(1.0f/(psamples*psamples));
			
			/*
			diffuse=diffuse*0.9f;
			
			
			//gathering specular light
						
			di=direction;
			di.Negate();
			
			f=2.0f * (normal*di);
			perturbated_normal=normal - di;
			perturbated_normal = perturbated_normal * f;
			perturbated_normal.Normalize();
			
			incoming=Ray(RayType::Specular,target_collision,perturbated_normal,target_triangle,depth + 1);
			
			specular = incoming * 0.1f;
			
			energy=(diffuse*material) + specular;
			*/
			
			energy=diffuse*material->Kd;
			break;
			
			
		}
	}
	else
	{
		/*
		ToDo:
		Get proper energy from sky/sunlight
		*/
		
		energy=sun_energy;
		
		
	}
	
	return energy;
	
}


#include "Core.hpp"
#include "MeshLoader.hpp"
#include "Spectrum.hpp"


#include <iostream>
#include <chrono>
#include <cmath>




using namespace std;
using namespace com::toxiclabs::iris;

Spectrum material;

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
	
	width=1024;
	height=768;
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
	
	/* create a default camera */
	Vector origin(2.0,6.0,-20.0);
	Vector target(0.0,0.0,0.0);
	Camera * camera = new Camera("camera-0",origin,target);
	scene.cameras.push_back(camera);
	
	scene.ApplyCamera();
	
	Spectrum spd("macbeth-5.spd");
	material=spd;
	cout<<"Spectrum:"<<endl<<spd.ToString()<<endl;
	
	ColorXYZ tmp;
	tmp=spd.ToXYZ();
	
	cout<<"xyz: "<<tmp.x<<","<<tmp.y<<","<<tmp.z<<endl;
		
		
}

Core::~Core()
{
	cout<<"[Core] closing..."<<endl;
	delete image;
}

void Core::Run()
{
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
	
	Spectrum stest;
	
	stest.data[20]=20.0f;
	ColorXYZ ctest = stest.ToXYZ();
	cout<<"XYZ:"<<ctest.x<<","<<ctest.y<<","<<ctest.z<<endl;
	ColorRGB rtest=ctest.ToRGB();
	cout<<"RGB:"<<rtest.r<<","<<rtest.g<<","<<rtest.b<<endl;
	
	image->save("out.png");
	
	cout<<"[Core] render finished"<<endl;
	
	
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
						RayCast(origin,direction,incoming);
												
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

void Core::RayCast(Vector & origin,Vector & direction,Spectrum & output)
{
	float min_dist=100000.0f;
	Vector collision;
	Vector oc;
	float dist;
	
	Vector target_collision;
	Triangle * target_triangle=nullptr;
	
	output.Clear();
	

	for(Triangle * triangle : scene.triangles)
	{
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
	
	
	if(target_triangle!=nullptr)
	{
		Vector w = target_collision - light;
		w.Normalize();
		
		float cosPhi = w * target_triangle->pnormal;
		
		output = material * cosPhi;
	}
}

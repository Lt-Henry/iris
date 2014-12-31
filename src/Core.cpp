
#include "Core.hpp"
#include "MeshLoader.hpp"
#include "Spectrum.hpp"


#include <iostream>
#include <chrono>
#include <cmath>




using namespace std;
using namespace com::toxiclabs::iris;



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
	
	
	Spectrum spd("VC_palik.k.spd");
	
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
			
			image->setPixelColor(i,j,&pixel);
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
	float Z=20.0f; //Hack camera Z coord
	float v = Z * tan(DegToRad(beta));
	float fwidth,fheight;
	float pw,ph;
	
	//far plane dimensions
	fheight = v * 2.0f;
	fwidth = fheight * 1.333f; // hardcoded aspect ratio
	 
	//pixel dimensions
	pw = fwidth/width;
	ph = fheight/height;
	
	Vector origin;
	Vector direction;

	Spectrum incoming;

	RenderChunk * chunk = GetChunk();
	while(chunk!=nullptr)
	{
		chunk->image=new ColorXYZ[chunk->w * chunk->h];
	
		for(int j=chunk->y;j<(chunk->y+chunk->h);j++)
		{
			for(int i=chunk->x;i<(chunk->x+chunk->w);i++)
			{
				origin.Set(0.0f,0.0f,-Z,1.0f);
				
				direction.Set
				((i*pw)+(pw*0.5f)-(width*pw*0.5f),
				(ph*0.5f)+(height*ph*0.5f)-(j*ph),5.0f,
				0.0f);
				
				direction = direction - origin;
				
				direction.Normalize();
				
				int px = i-chunk->x;
				int py = j-chunk->y;
				
				//chunk->image[px+py*chunk->w].Black();
				
				RayCast(origin,direction,incoming);
				
				chunk->image[px+py*chunk->w]=incoming.ToXYZ();
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
				output.data[2]=1.0f-(dist/25.0);
			}
		}				
	}
}

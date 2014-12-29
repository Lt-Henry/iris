
#include <iostream>
#include <chrono>

#include "Core.hpp"
#include "MeshLoader.hpp"
#include "Spectrum.hpp"

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
	
	screen_w=1024;
	screen_h=768;
	num_threads=2;
	
	/* computing chunks */
	int wchunk=60;
	int hchunk=60;
		
	while(screen_w%wchunk!=0)
	{
		wchunk++;
	}
	
	while(screen_h%hchunk!=0)
	{
		hchunk++;
	}
	
	for(int i=0;i<(screen_w/wchunk);i++)
	{
		for(int j=0;j<(screen_h/hchunk);j++)
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
	
	Color tmp;
	spd.ToXYZ(&tmp);
	tmp=tmp.XYZtoRGB();
	cout<<"rgb: "<<tmp.r<<","<<tmp.g<<","<<tmp.b<<endl;
		
		
}

Core::~Core()
{
	cout<<"[Core] closing..."<<endl;
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
	
	chunk_mutex.unlock();
}

void Core::RenderThread(int id)
{
	cout<<"[Core] Thread "<<id<<" entered rendering"<<endl;

	RenderChunk * chunk = GetChunk();
	while(chunk!=nullptr)
	{
		/* render goes here */
		std::chrono::milliseconds dura(50);
		std::this_thread::sleep_for( dura );
		CommitChunk(chunk);
		chunk = GetChunk();
	}

	cout<<"[Core] Thread "<<id<<" exit rendering"<<endl;
}

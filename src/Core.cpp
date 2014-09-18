
#include <iostream>
#include <chrono>
#include "Core.hpp"
#include "MeshLoader.hpp"

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
	
	/* fake chunks */
	
	chunks.push_back(new RenderChunk());
	chunks.push_back(new RenderChunk());
	chunks.push_back(new RenderChunk());
		
		
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
	cout<<"[Core] Chunk "<<chunk->x<<","<<chunk->y<<" commited"<<endl;
}

void Core::RenderThread(int id)
{
	cout<<"[Core] Thread "<<id<<" entered rendering"<<endl;

	RenderChunk * chunk = GetChunk();
	while(chunk!=nullptr)
	{
		/* render goes here */
		std::chrono::milliseconds dura(3000);
		std::this_thread::sleep_for( dura );
		CommitChunk(chunk);
		chunk = GetChunk();
	}

	cout<<"[Core] Thread "<<id<<" exit rendering"<<endl;
}


#include <iostream>
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
		MeshLoader::Load(argv[1]);
	}
	
	
	/* default render settings */
	
	screen_w=1024;
	screen_h=768;
	num_threads=2;
}

Core::~Core()
{
	cout<<"[Core] closing..."<<endl;
}

void Core::Run()
{
	cout<<"[Core] running..."<<endl;
	
}

RenderChunk Core::GetChunk()
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
	}

	cout<<"[Core] Thread "<<id<<" exit rendering"<<endl;
}



#ifndef _IRIS_CORE_
#define _IRIS_CORE_

#include <vector>
#include <thread>
#include <mutex>
#include "Render.hpp"


namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Core
			{
				public:
				
				std::vector<RenderChunk *> chunks;
				std::mutex chunk_mutex;
				
				/* render settings */
				int screen_w;
				int screen_h;
				int num_threads;
				
				/**
				 * constructor
				 */ 
				Core(int argc,char * argv[]);
				
				/**
				 * destructor
				 */ 
				~Core();
				
				/**
				 * Starts the system
				 */ 
				void Run();
				
				/**
				* Request a new chunk to render. Thread safe	
				*/
				RenderChunk * GetChunk();		
				
				/**
				* Commits an already rendered chunk. Thread safe
				*/
				void CommitChunk(RenderChunk * chunk);
				
				/**
				* Render thread method
				*/
				void RenderThread(int id);
			};
		}
	}
}


#endif

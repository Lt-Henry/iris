

#ifndef _IRIS_CORE_
#define _IRIS_CORE_

#include "Render.hpp"
#include "Scene.hpp"

#include <vector>
#include <thread>
#include <mutex>

#include <FreeImagePlus.h>


namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Core
			{
				public:
				
				Scene scene;
				
				std::vector<RenderChunk *> chunks;
				std::mutex chunk_mutex;
				
				/* render settings */
				int width;
				int height;
				int num_threads;
				
				/* render target */
				fipImage * image;
				
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



#ifndef _IRIS_CORE_
#define _IRIS_CORE_

#include "Render.hpp"
#include "Scene.hpp"
#include "Spectrum.hpp"
#include "Math.hpp"
#include "KdTree.hpp"

#include <vector>
#include <thread>
#include <mutex>

#include <gsl/gsl_qrng.h>
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
				KdTree * tree;
				
				std::vector<RenderChunk *> chunks;
				std::mutex chunk_mutex;
				
				/* render settings */
				int width;
				int height;
				int num_threads;
				int samples;
				
				/* render target */
				fipImage * image;
				
				/* random generator */
				gsl_qrng * qr[8];
				
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
				
				/**
				* Cast a visibility ray
				*/
				void RayCast(int id,Vector & origin,Vector & direction,Spectrum & output); 
				
				Spectrum PathTrace(Vector & origin, Vector & direction,Triangle * source,int depth=1);
			};
		}
	}
}


#endif

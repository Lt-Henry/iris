

#ifndef _IRIS_PATHTRACER_
#define _IRIS_PATHTRACER_

#include "Render.hpp"
#include "Scene.hpp"
#include "Spectrum.hpp"
#include "Math.hpp"
#include "KdTree.hpp"
#include "Atmosphere.hpp"

#include <vector>
#include <thread>
#include <mutex>
#include <functional>

#include <FreeImagePlus.h>


namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			enum class RayType
			{
				Visibility, Diffuse, Specular, Glossy, Subsurface
			};
		
			class PathTracer
			{
			
				private:
				
				std::function<void(float)> progress;
				
				int total_chunks;
			
				public:
				
				
				
				Scene scene;
				KdTree * tree;
				
				std::vector<RenderChunk *> chunks;
				std::mutex chunk_mutex;
				
				/* image settings */
				int width;
				int height;
				int samples;
				
				/* cpu settings */
				int num_threads;
				
				/* pathtracer settings */
				int psamples;
				
				/* render target */
				fipImage * image;
				
				
				/**
				 * constructor
				 */ 
				PathTracer(Scene & scene);
				
				/**
				 * destructor
				 */ 
				~PathTracer();
				
				/**
				 * Starts the system
				 */ 
				void Run(std::function<void(float)> progress);
				
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
				
				//Spectrum PathTrace(Vector & origin, Vector & direction,Geometry * source,int depth=1);
				
				Spectrum Ray(RayType type,Vector & origin,Vector & direction,
				Geometry * source=nullptr,int depth=1);
				
			};
		}
	}
}


#endif

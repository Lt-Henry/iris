

#ifndef _IRIS_MESH_
#define _IRIS_MESH_

#include "Math.hpp"

#include <array>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
		
			class BoundBox
			{
				public:
				
				Vector min;
				Vector max;
				
				
			};
			
			class Triangle
			{
				public:
				/*
				std::array<Vector,3> vertices;
				std::array<Vector,3> normals;
				*/
				Vector vertices[3];
				Vector normals[3];
				
				/* computed normal, used for backface culling */
				Vector pnormal;
				
				int material;
				
				/* precomputed D */
				float D; 
				
				bool RayCollision(Vector & origin,Vector & direction,Vector & collision);
				
				void Print();
				
				Vector GetCentroid();
				BoundBox GetBoundBox();
				
				Vector PerturbateNormal(float angle,float r1,float r2);
				Vector GetUVW(Vector & collision);
				Vector GetAveragedNormal(Vector & collision);
			};
			
			BoundBox operator+(BoundBox & a,BoundBox & b);
		}
	}
}


#endif

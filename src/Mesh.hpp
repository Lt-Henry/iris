

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
				
				bool RayCollision(Vector & origin,Vector & direction,Vector & collision);
				
				void Print();
				
				Vector GetCentroid();
				BoundBox GetBoundBox();
			};
			
			BoundBox operator+(BoundBox & a,BoundBox & b);
		}
	}
}


#endif



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
				
				bool infinite;
				
				Vector min;
				Vector max;
				
				BoundBox();
				BoundBox(Vector & min,Vector & max);
				
				
			};
			
			class Geometry
			{
				public:
				
				virtual bool RayCollision(Vector & origin,Vector & direction,Vector & collision)=0;
				virtual Vector GetCentroid()=0;
				virtual BoundBox GetBoundBox()=0;
				
				virtual Vector PerturbateNormal(float angle,float r1,float r2)=0;
				virtual Vector GetUVW(Vector & collision)=0;
				virtual Vector GetAveragedNormal(Vector & collision)=0;
				
				virtual void Mult(Matrix & m)=0;
				
				virtual std::string ToString()=0;
			};
			
			
			class Plane: public Geometry
			{
				public:
				
				Vector base;
				Vector coplanar;
				Vector normal;
				float height;
				
				Plane(float height);
				
				bool RayCollision(Vector & origin,Vector & direction,Vector & collision);
				Vector GetCentroid();
				BoundBox GetBoundBox();
				
				Vector PerturbateNormal(float angle,float r1,float r2);
				Vector GetUVW(Vector & collision);
				Vector GetAveragedNormal(Vector & collision);
				
				void Mult(Matrix & m);
				
				std::string ToString();
			};
			
			class Triangle : public Geometry
			{
				public:
				
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
				
				void Mult(Matrix & m);
				
				std::string ToString();
			};
			
			BoundBox operator+(BoundBox & a,BoundBox & b);
		}
	}
}


#endif

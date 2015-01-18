
#ifndef _IRIS_MATH_
#define _IRIS_MATH_

#include <stdint.h>

#define EPSILON 0.000001

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			
			float DegToRad(float degree);
			float RadToDeg(float rad);
				
			bool AproxToZero(float v);
			float Maxf(float a,float b);
							
			/*!
				Simple homogeneus matrix class
			 */ 
			class Matrix
			{
				public:
					float data[16];
					/*!
					  Creates a new identity matrix 
					 */
					static Matrix CreateIdentity();
					
					/*!
						Creates a new translation matrix
					 */ 
					static Matrix CreateTranslation(float x,float y,float z);
			};
			
			/*!
				Homogeneus Vector class
			*/
			class Vector
			{
				public:
					union
					{
						struct
						{
							float x;
							float y;
							float z;
							float w;
						};
						float data[4];
					};
					
					/*!
						Empty constructor
					*/
					Vector();
					
					/*!
						XYZW constructor
					*/
					Vector(float x,float y,float z,float w=0.0f);
					
					/*!
						Sets values
					*/
					void Set(float x,float y,float z,float w);
					
					/*! 
						Returns vector module (euclidean distance)
					*/
					float Module();
					
					/*!
						Negates vector
					*/
					void Negate();
					
					/*!
						Normalizes the vector
					*/
					void Normalize();
					
					/*!
						Convert the vector to an homogeneus one
					*/
					void Homogeneus();
					
					
					/*!
						Perform an Abs operation on all vector members
					 */ 
					void Abs();
					
					
					/*!
						print the vector to the stdout
					*/
					void Print();
					
					/*!
						Access  members
					*/
					inline float& operator[] (int n)
					{
						return data[n];
					}
				
			};
			
			/*!
				vector addition
			*/
			Vector operator+(Vector  a,Vector &b);
			
			/*!
				vector substraction
			*/
			Vector operator-(Vector a,Vector &b);  
			
			/*!
				vector scalar product
			*/
			float operator*(Vector &a,Vector &b);
			
			/*!
				vector product
			*/
			Vector operator^(Vector &a,Vector &b);
			
			/*!
				Vector and scalar product
			*/
			Vector operator*(Vector &a,float s);
			
			/*!
				Vector and Matrix product
			 */ 
			Vector operator*(Vector &v,Matrix &m);
			
			/*!
				Matrix - Matrix product
			 */ 
			Matrix operator*(Matrix &a,Matrix &b);
			
			/*!
				Vector Linear interpolation
			 */ 
			Vector Lerp(Vector &a,Vector &b,float factor);
		}
	}
}



#endif

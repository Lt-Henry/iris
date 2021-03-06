/*
	Iris raytracer
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _IRIS_MATH_
#define _IRIS_MATH_

#include <cstdint>
#include <string>
#include <cmath>

#ifdef IRIS_X86_MATH

#include <xmmintrin.h>
#include <pmmintrin.h>

#ifdef IRIS_SSE4
#include <smmintrin.h>
#endif

#endif

#define EPSILON 0.000001
#define INV_PI (1.0f/M_PI)

namespace Math
{
	/*
		degrees to radians
	*/
	inline float radians(float degree)
	{
		return degree*0.01745329251f; //Pi/180.0
	}
	
	/*
		radians to degrees
	*/
	inline float degrees(float rad)
	{
		return (rad*180.0f)/M_PI;
	}
	
	/*
		radians to degrees in the [0,360] range
	*/
	inline float degrees_nice(float rad)
	{
		float deg=degrees(rad)/360.0f;
		
		int ideg=(int)deg;
		
		float fraction=deg-ideg;
		
		return 360.0f*fraction;
	}
	
	/*
		Decibels to linear
	*/
	inline float linear(float db)
	{
		return (std::pow(10.0f,db/20.0f));
	}

	inline bool is_zero(float v)
	{
#ifdef IRIS_X86_MATH
		static const __m128 E=_mm_set1_ps(EPSILON);
		/*
		abs code credits:
		http://fastcpp.blogspot.com.es/2011/03/changing-sign-of-float-values-using-sse.html
		*/
		static const __m128 SIGNMASK =
			_mm_castsi128_ps(_mm_set1_epi32(0x80000000));
		__m128 V; 
		__m128 absval;
		__m128 R;
		int ret;
		 
		V=_mm_set1_ps(v);
		absval= _mm_andnot_ps(SIGNMASK, V); // absval = abs(val)
		R=_mm_cmplt_ss(absval,E);
		
		ret=_mm_extract_epi32(_mm_castps_si128(R),0);
		
		return (bool)ret;
		
#else

		float t = std::abs(0.0f - v);
		
		return (t<EPSILON);
#endif
	}
	
	inline float max(float a,float b)
	{
#ifdef IRIS_X86_MATH

		__m128 A;
		__m128 B;
		__m128 R;
		float ret;
		A=_mm_set1_ps(a);
		B=_mm_set1_ps(b);
		R=_mm_max_ss(A,B);
		//_mm_store_ss(&ret,R);
		ret=_mm_cvtss_f32(R);
		return ret;

#else

		if (a>b) {
			return a;
		}
		else {
			return b;
		}
		
#endif
	}
	
	
	/*
		Simple homogeneus matrix class
	*/ 
	class Matrix {
	public:
	
		
		float data[16]; //matrix data
		
		/*
			Creates a new identity matrix 
		*/
		static Matrix identity();

		/*
			Creates a new translation matrix
		*/ 
		static Matrix translation(float x,float y,float z);
	};
	
	
	
	/*
		Homogeneus Vector class
	*/
	class Vector {
	public:
	
		/*
			vector components
		*/
		union {
			struct {
				float x;
				float y;
				float z;
				float w;
			};
#ifdef IRIS_X86_MATH
			__m128 data;
#else
			float data[4];
#endif
		};
		
		/*
			Empty constructor
		*/
		Vector();
	
		/*
			XYZW constructor
		*/
		Vector(float x,float y,float z,float w=0.0f);
	
		/*
			Sets values
		*/
		void set(float x,float y,float z,float w);
	
		/*
			Returns vector module (euclidean distance)
		*/
		float module();
	
		/*
			Negates vector
		*/
		void negate();
	
		/*
			Normalizes the vector
		*/
		void normalize();
	
		/*
			Convert the vector to an homogeneus one
		*/
		void homogeneus();
	
		/*
			Perform an Abs operation on all vector members
		*/
		void abs();
	
		/*
			String representation
		*/
		std::string to_string();
	
		/*
			Members access
		*/
		inline float& operator[] (int n)
		{
#ifdef IRIS_X86_MATH
			return ((float*)&data)[n];
#else
			return data[n];
#endif
		}

	};
	
	/*
		Point (just a Vector with homogeneus component equal to 1)
	*/
	class Point: public Vector {
	public:
	
		Point(float x,float y,float z);
	};
	
	
	
	/*
		vector addition
	*/
	Vector operator+(Vector  a,Vector &b);

	/*
		vector substraction
	*/
	Vector operator-(Vector a,Vector &b);  

	/*
		vector scalar product
	*/
	float operator*(Vector &a,Vector &b);

	/*
		vector product
	*/
	Vector operator^(Vector &a,Vector &b);

	/*
		Vector and scalar product
	*/
	Vector operator*(Vector &a,float s);

	/*
		Vector and Matrix product
	*/
	Vector operator*(Vector &v,Matrix &m);

	/*
		Matrix - Matrix product
	*/
	Matrix operator*(Matrix &a,Matrix &b);

	/*
		Vector Linear interpolation
	*/
	Vector lerp(Vector &a,Vector &b,float factor);
}



#endif

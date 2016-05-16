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

#include "Math.hpp"

#include <cmath>
#include <cstring>
#include <unistd.h>
#include <iostream>

using namespace std;


namespace Math
{
	Matrix Matrix::identity()
	{
		Matrix m;
	
		memset(m.data,0,sizeof(float)*16);
	
		m.data[0]=1.0f;
		m.data[5]=1.0f;
		m.data[10]=1.0f;
		m.data[15]=1.0f;
	
		return m;
	}

	Matrix Matrix::translation(float x,float y,float z)
	{
		Matrix m = identity();
		
		m.data[12]=x;
		m.data[13]=y;
		m.data[14]=z;
		
		return m;
	}


	Vector::Vector()
	{
		
	}
		
			
	Vector::Vector(float x,float y,float z,float w)
	{
		set(x,y,z,w);
	}
		
			
	void Vector::set(float x,float y,float z,float w=0.0f)
	{
		data[0]=x;
		data[1]=y;
		data[2]=z;
		data[3]=w;
	}
	
	
	float Vector::module()
	{

#ifdef IRIS_X86_MATH

		float dist;
		__m128 R;
		
		
		R=_mm_dp_ps(data, data, 0x71);
		R=_mm_sqrt_ss(R);
		dist=_mm_cvtss_f32(R);
							
		return dist;

#else
		float t;
		
		t=(x*x)+(y*y)+(z*z)+(w*w);
		
		return std::sqrt(t);
#endif				

	}
	
	
	void Vector::negate()
	{
		data[0]=-data[0];
		data[1]=-data[1];
		data[2]=-data[2];
		data[3]=-data[3];
	}
	
	
	void Vector::normalize()
	{

#ifdef IRIS_X86_MATH

		static const __m128 ONE=_mm_set1_ps(1.0f);
	
		__m128 R;
		__m128 S;
		float s;
					
		S=_mm_dp_ps(data, data, 0x71);
		S=_mm_sqrt_ss(S);
		S=_mm_div_ss(ONE,S);
		//_mm_store_ss(&s,S);
		s=_mm_cvtss_f32(S);
	
		S=_mm_set1_ps(s);//broadcast content to four elements
		data=_mm_mul_ps(data,S);
#else

		float module = 1.0f/module();
		x=x*module;
		y=y*module;
		z=z*module;
		w=w*module;

#endif
	
	}
	
	
	
	void Vector::homogeneus()
	{
		float f=1.0f/data[3];
		
		data[0]=data[0]*f;
		data[1]=data[1]*f;
		data[2]=data[2]*f;
		data[3]=data[3]*f;
	}
	
	
	
	void Vector::abs()
	{

#ifdef IRIS_X86_MATH

		static const __m128 SIGNMASK =
			_mm_castsi128_ps(_mm_set1_epi32(0x80000000));
		__m128 V; 
		__m128 absval;
		 
		 
		//V=_mm_loadu_ps(data);
		//absval= _mm_andnot_ps(SIGNMASK, V); // absval = abs(val)
		data=_mm_andnot_ps(SIGNMASK,data);
		//_mm_storeu_ps(data,absval);
		
#else
		data[0]=std::abs(data[0]);
		data[1]=std::abs(data[1]);
		data[2]=std::abs(data[2]);
		data[3]=std::abs(data[3]);
		
#endif
		
	}
	
	
	
	string Vector::to_string()
	{
		string result;
		
		result="("+std::to_string(x)+","+std::to_string(y)+","+std::to_string(z)+","+std::to_string(w)+")";
		return result;
	}
	
	
	
	Vector operator+(Vector a,Vector &b)
	{
#ifdef IRIS_X86_MATH
		
		Vector v;
		v.data=_mm_add_ps(a.data,b.data);
		return v;
#else
		
		Vector v;
		
		v.data[0]=a.data[0]+b.data[0];
		v.data[1]=a.data[1]+b.data[1];
		v.data[2]=a.data[2]+b.data[2];
		v.data[3]=a.data[3]+b.data[3];
		
		return v;

#endif
	}
	
	
	
	Vector operator-(Vector a,Vector &b)
	{
		
#ifdef IRIS_X86_MATH

		Vector v;
		v.data=_mm_sub_ps(a.data,b.data);
		return v;
		
#else
		Vector v;
		
		v.data[0]=a.data[0]-b.data[0];
		v.data[1]=a.data[1]-b.data[1];
		v.data[2]=a.data[2]-b.data[2];
		v.data[3]=a.data[3]-b.data[3];
		
		return v;
		
#endif

	}
	
	
	
	float operator*(Vector &a,Vector &b)
	{
		
#ifdef IRIS_X86_MATH
#ifdef IRIS_SSE4
		float ret;
				
		__m128 R;
		/* mask: 0111 0001 */
		R=_mm_dp_ps(a.data,b.data,0x71);
		_mm_store_ss(&ret,R);
		return ret;
	
#else
		float ret;
		__m128 A;
		__m128 B;
		__m128 R1;
		__m128 R2;
		__m128 R3;
		A=_mm_set_ps(a.data[0],a.data[1],a.data[2],0.0f);
		B=_mm_set_ps(b.data[0],b.data[1],b.data[2],0.0f);
		R1 = _mm_mul_ps(A, B);
		R2 = _mm_hadd_ps(R1, R1);
		R3 = _mm_hadd_ps(R2, R2);
		_mm_store_ss(&ret, R3);
		
		return ret;
#endif

#else
		float ret;
	
		ret=(a.data[0]*b.data[0]) + (a.data[1]*b.data[1]) + (a.data[2]*b.data[2]) + (a.data[3]*b.data[3]);
	
		return ret;
#endif
	}
	
	
	
	Vector operator^(Vector &a,Vector &b)
	{
		Vector ret;
		
		//TODO: complete formula for w
		ret.data[0] = a.data[1]*b.data[2] - a.data[2]*b.data[1];
		ret.data[1] = a.data[2]*b.data[0] - a.data[0]*b.data[2];
		ret.data[2] = a.data[0]*b.data[1] - a.data[1]*b.data[0];
		ret.data[3]=0.0f;
		
		return ret;
	}
	
	
	Vector operator*(Vector &a,float s)
	{
#ifdef IRIS_X86_MATH
		Vector ret;
	
		__m128 F;
		F=_mm_set1_ps(s);
		ret.data=_mm_mul_ps(a.data,F);
		
		return ret;
#else
		Vector ret;
		
		ret.data[0]=a.data[0]*s;
		ret.data[1]=a.data[1]*s;
		ret.data[2]=a.data[2]*s;
		ret.data[3]=a.data[3]*s;
		
		return ret;
#endif
	}


			
	Vector operator*(Vector &v,Matrix &m)
	{
		Vector ret;
		
		ret.data[0]=v.data[0]*m.data[0] + v.data[1]*m.data[4] + v.data[2]*m.data[8] + v.data[3]*m.data[12];
		ret.data[1]=v.data[0]*m.data[1] + v.data[1]*m.data[5] + v.data[2]*m.data[9] + v.data[3]*m.data[13];
		ret.data[2]=v.data[0]*m.data[2] + v.data[1]*m.data[6] + v.data[2]*m.data[10] + v.data[3]*m.data[14];
		ret.data[3]=v.data[0]*m.data[3] + v.data[1]*m.data[7] + v.data[2]*m.data[11] + v.data[3]*m.data[15];
				 
		return ret;
	}


	
	
	Matrix operator*(Matrix &a,Matrix &b)
	{
		Matrix m;
		
		//Holy shit!
		
		m.data[0 + 0*4]=a.data[0 + 0*4]*b.data[0 + 0*4] + a.data[0 + 1*4]*b.data[1 + 0*4] + a.data[0 + 2*4]*b.data[2 + 0*4] + a.data[0 + 3*4]*b.data[3 + 0*4];
		m.data[1 + 0*4]=a.data[1 + 0*4]*b.data[0 + 0*4] + a.data[1 + 1*4]*b.data[1 + 0*4] + a.data[1 + 2*4]*b.data[2 + 0*4] + a.data[1 + 3*4]*b.data[3 + 0*4];
		m.data[2 + 0*4]=a.data[2 + 0*4]*b.data[0 + 0*4] + a.data[2 + 1*4]*b.data[1 + 0*4] + a.data[2 + 2*4]*b.data[2 + 0*4] + a.data[2 + 3*4]*b.data[3 + 0*4];
		m.data[3 + 0*4]=a.data[3 + 0*4]*b.data[0 + 0*4] + a.data[3 + 1*4]*b.data[1 + 0*4] + a.data[3 + 2*4]*b.data[2 + 0*4] + a.data[3 + 3*4]*b.data[3 + 0*4];
		
		m.data[0 + 1*4]=a.data[0 + 0*4]*b.data[0 + 1*4] + a.data[0 + 1*4]*b.data[1 + 1*4] + a.data[0 + 2*4]*b.data[2 + 1*4] + a.data[0 + 3*4]*b.data[3 + 1*4];
		m.data[1 + 1*4]=a.data[1 + 0*4]*b.data[0 + 1*4] + a.data[1 + 1*4]*b.data[1 + 1*4] + a.data[1 + 2*4]*b.data[2 + 1*4] + a.data[1 + 3*4]*b.data[3 + 1*4];
		m.data[2 + 1*4]=a.data[2 + 0*4]*b.data[0 + 1*4] + a.data[2 + 1*4]*b.data[1 + 1*4] + a.data[2 + 2*4]*b.data[2 + 1*4] + a.data[2 + 3*4]*b.data[3 + 1*4];
		m.data[3 + 1*4]=a.data[3 + 0*4]*b.data[0 + 1*4] + a.data[3 + 1*4]*b.data[1 + 1*4] + a.data[3 + 2*4]*b.data[2 + 1*4] + a.data[3 + 3*4]*b.data[3 + 1*4];
		
		m.data[0 + 2*4]=a.data[0 + 0*4]*b.data[0 + 2*4] + a.data[0 + 1*4]*b.data[1 + 2*4] + a.data[0 + 2*4]*b.data[2 + 2*4] + a.data[0 + 3*4]*b.data[3 + 2*4];
		m.data[1 + 2*4]=a.data[1 + 0*4]*b.data[0 + 2*4] + a.data[1 + 1*4]*b.data[1 + 2*4] + a.data[1 + 2*4]*b.data[2 + 2*4] + a.data[1 + 3*4]*b.data[3 + 2*4];
		m.data[2 + 2*4]=a.data[2 + 0*4]*b.data[0 + 2*4] + a.data[2 + 1*4]*b.data[1 + 2*4] + a.data[2 + 2*4]*b.data[2 + 2*4] + a.data[2 + 3*4]*b.data[3 + 2*4];
		m.data[3 + 2*4]=a.data[3 + 0*4]*b.data[0 + 2*4] + a.data[3 + 1*4]*b.data[1 + 2*4] + a.data[3 + 2*4]*b.data[2 + 2*4] + a.data[3 + 3*4]*b.data[3 + 2*4];
		
		m.data[0 + 3*4]=a.data[0 + 0*4]*b.data[0 + 3*4] + a.data[0 + 1*4]*b.data[1 + 3*4] + a.data[0 + 2*4]*b.data[2 + 3*4] + a.data[0 + 3*4]*b.data[3 + 3*4];
		m.data[1 + 3*4]=a.data[1 + 0*4]*b.data[0 + 3*4] + a.data[1 + 1*4]*b.data[1 + 3*4] + a.data[1 + 2*4]*b.data[2 + 3*4] + a.data[1 + 3*4]*b.data[3 + 3*4];
		m.data[2 + 3*4]=a.data[2 + 0*4]*b.data[0 + 3*4] + a.data[2 + 1*4]*b.data[1 + 3*4] + a.data[2 + 2*4]*b.data[2 + 3*4] + a.data[2 + 3*4]*b.data[3 + 3*4];
		m.data[3 + 3*4]=a.data[3 + 0*4]*b.data[0 + 3*4] + a.data[3 + 1*4]*b.data[1 + 3*4] + a.data[3 + 2*4]*b.data[2 + 3*4] + a.data[3 + 3*4]*b.data[3 + 3*4];
		
		
		return m;
	}
	
	
	
	Vector lerp(Vector &a,Vector &b,float factor)
	{
		Vector ret;
		
		ret.data[0]=a.data[0] + factor*(b.data[0]-a.data[0]);
		ret.data[1]=a.data[1] + factor*(b.data[1]-a.data[1]);
		ret.data[2]=a.data[2] + factor*(b.data[2]-a.data[2]);
		ret.data[3]=a.data[3] + factor*(b.data[3]-a.data[3]);
		
		return ret;
	}

}




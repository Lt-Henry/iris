
#include "Math.hpp"
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <iostream>





namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			
			Matrix Matrix::CreateIdentity()
			{
				Matrix m;
				
				memset(m.data,0,sizeof(float)*16);
				
				m.data[0]=1.0f;
				m.data[5]=1.0f;
				m.data[10]=1.0f;
				m.data[15]=1.0f;
				
				return m;
			}
			
			Matrix Matrix::CreateTranslation(float x,float y,float z)
			{
				Matrix m = CreateIdentity();
								
				
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
				Set(x,y,z,w);
			}
			
			void Vector::Set(float x,float y,float z,float w=0.0f)
			{
				data[0]=x;
				data[1]=y;
				data[2]=z;
				data[3]=w;
			}
			
			float Vector::Module()
			{
		
				
				float dist;
				__m128 R;
				
				
				R=_mm_dp_ps(data, data, 0x71);
				R=_mm_sqrt_ss(R);
				dist=_mm_cvtss_f32(R);
									
				return dist;
		
			}
			
			void Vector::Negate()
			{
				data[0]=-data[0];
				data[1]=-data[1];
				data[2]=-data[2];
				data[3]=-data[3];
			}
			
			void Vector::Normalize()
			{
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
			
			}
			
			void Vector::Homogeneus()
			{
				float f=1.0f/data[3];
				
				data[0]=data[0]*f;
				data[1]=data[1]*f;
				data[2]=data[2]*f;
				data[3]=data[3]*f;
			}
			
			void Vector::Abs()
			{
				static const __m128 SIGNMASK =
					_mm_castsi128_ps(_mm_set1_epi32(0x80000000));
				__m128 V; 
				__m128 absval;
				 
				 
				//V=_mm_loadu_ps(data);
				//absval= _mm_andnot_ps(SIGNMASK, V); // absval = abs(val)
				data=_mm_andnot_ps(SIGNMASK,data);
				//_mm_storeu_ps(data,absval);
				
			}
			
			void Vector::Print()
			{
				std::cout<<"vector:"<<data[0]<<","<<data[1]<<","<<data[2]<<","<<data[3]<<std::endl;
			}
						
			
			Vector operator+(Vector  a,Vector &b)
			{
				
				Vector v;
				v.data=_mm_add_ps(a.data,b.data);
				return v;
			}
			
			Vector operator-(Vector  a,Vector &b)
			{
				Vector v;
				v.data=_mm_sub_ps(a.data,b.data);
				return v;
			}
			
			float operator*(Vector &a,Vector &b)
			{
#ifdef _IRIS_SSE4_
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
			}
			
			Vector operator^(Vector &a,Vector &b)
			{
				Vector r;
				
				r.data[0] = a.data[1]*b.data[2] - a.data[2]*b.data[1];
				r.data[1] = a.data[2]*b.data[0] - a.data[0]*b.data[2];
				r.data[2] = a.data[0]*b.data[1] - a.data[1]*b.data[0];
				r.data[3]=0.0f;
				
				return r;
			}
			
			Vector operator*(Vector &a,float s)
			{
				Vector r;
			
				__m128 F;
				F=_mm_set1_ps(s);
				r.data=_mm_mul_ps(a.data,F);
				
				return r;
			}
			
			Vector operator*(Vector &v,Matrix &m)
			{
				Vector r;
				
				r.data[0]=v.data[0]*m.data[0] + v.data[1]*m.data[4] + v.data[2]*m.data[8] + v.data[3]*m.data[12];
				r.data[1]=v.data[0]*m.data[1] + v.data[1]*m.data[5] + v.data[2]*m.data[9] + v.data[3]*m.data[13];
				r.data[2]=v.data[0]*m.data[2] + v.data[1]*m.data[6] + v.data[2]*m.data[10] + v.data[3]*m.data[14];
				r.data[3]=v.data[0]*m.data[3] + v.data[1]*m.data[7] + v.data[2]*m.data[11] + v.data[3]*m.data[15];
						 
				return r;
			}
			
			Matrix operator*(Matrix &a,Matrix &b)
			{
				Matrix m;
				
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
			
			Vector Lerp(Vector &a,Vector &b,float factor)
			{
				Vector r;
				
				r.data[0]=a.data[0] + factor*(b.data[0]-a.data[0]);
				r.data[1]=a.data[1] + factor*(b.data[1]-a.data[1]);
				r.data[2]=a.data[2] + factor*(b.data[2]-a.data[2]);
				r.data[3]=a.data[3] + factor*(b.data[3]-a.data[3]);
									
				return r;
			}
		}
	}
}

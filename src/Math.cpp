
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
			
			float DegToRad(float degree)
			{
				/*
					PI/180=0.01745329251
				*/
				return degree*0.01745329251f;
			}

			float RadToDeg(float rad)
			{
				return (rad*180.0f)/M_PI;
			}
			
			
			
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
				this->x=x;
				this->y=y;
				this->z=z;
				this->w=w;
			}
			
			float Vector::Module()
			{
			#ifdef _IRIS_SSE_
				#warning "Using SSE Module"
				
				float dist;
				__m128 R;
				__m128 V;
				
				V=_mm_loadu_ps(data);
				R=_mm_dp_ps(V, V, 0x71);
				R=_mm_sqrt_ss(R);
				dist=_mm_cvtss_f32(R);
				//_mm_storeu_ps(dist,R);
									
				return dist;
			#else
			
				float dist = std::sqrt((data[0]*data[0])+(data[1]*data[1])+(data[2]*data[2])+(data[3]*data[3]));
				
				return dist;
			#endif
			}
			
			void Vector::Negate()
			{
				x=-x;
				y=-y;
				z=-z;
				w=-w;
			}
			
			void Vector::Normalize()
			{
			#ifdef _IRIS_SSE_
				static const __m128 ONE=_mm_set1_ps(1.0f);
				__m128 V;
				__m128 R;
				__m128 S;
				float s;
								
				V=_mm_loadu_ps(data);
				S=_mm_dp_ps(V, V, 0x71);
				S=_mm_sqrt_ss(S);
				S=_mm_div_ss(ONE,S);
				//_mm_store_ss(&s,S);
				s=_mm_cvtss_f32(S);
				
				S=_mm_set1_ps(s);//broadcast content to four elements
				R=_mm_mul_ps(V,S);
				_mm_storeu_ps(data,R);
			#else
				float tmp = (x*x)+(y*y)+(z*z)+(w*w);
				float scale = 1.0f/std::sqrt(tmp);
				
				x=x*scale;
				y=y*scale;
				z=z*scale;
				w=w*scale;
			#endif
								
			}
			
			void Vector::Homogeneus()
			{
				float f=1.0f/w;
				
				x=x*f;
				y=y*f;
				z=z*f;
				w=w*f;
			}
			
			void Vector::Abs()
			{
			#ifdef _IRIS_SSE_
				static const __m128 SIGNMASK =
					_mm_castsi128_ps(_mm_set1_epi32(0x80000000));
				__m128 V; 
				__m128 absval;
				 
				V=_mm_loadu_ps(data);
				absval= _mm_andnot_ps(SIGNMASK, V); // absval = abs(val)
				
				_mm_storeu_ps(data,absval);
				
			#else
				x=fabs(x);
				y=fabs(y);
				z=fabs(z);
				w=fabs(w);
			#endif
			}
			
			void Vector::Print()
			{
				std::cout<<"vector:"<<x<<","<<y<<","<<z<<","<<w<<std::endl;
			}
						
			
			Vector operator+(Vector  a,Vector &b)
			{
			#ifdef _IRIS_SSE_
				Vector v;
			
				__m128 A;
				__m128 B;
				__m128 R;
				A=_mm_loadu_ps(a.data);
				B=_mm_loadu_ps(b.data);
				R=_mm_add_ps(A,B);
				_mm_storeu_ps(v.data,R);
			
				return v;
			#else
				Vector v(a.data[0]+b.data[0],a.data[1]+b.data[1],a.data[2]+b.data[2],a.data[3]+b.data[3]);
			
				return v;
			#endif
				
			}
			
			Vector operator-(Vector  a,Vector &b)
			{
			#ifdef _IRIS_SSE_
				Vector v;
			
				__m128 A;
				__m128 B;
				__m128 R;
				A=_mm_loadu_ps(a.data);
				B=_mm_loadu_ps(b.data);
				R=_mm_sub_ps(A,B);
				_mm_storeu_ps(v.data,R);
			
				return v;
			
			#else
				Vector v(a.data[0]-b.data[0],a.data[1]-b.data[1],a.data[2]-b.data[2],a.data[3]-b.data[3]);
			
				return v;
			#endif
				
				
			}
			
			float operator*(Vector &a,Vector &b)
			{
			#ifdef _IRIS_SSE_
				#ifdef _IRIS_SSE4_
					#warning "Using SSE4.1 dot product"
					float ret;
									
					__m128 A;
					__m128 B;
					__m128 R;
					A=_mm_loadu_ps(a.data);
					B=_mm_loadu_ps(b.data);
					/* mask: 0111 0001 */
					R=_mm_dp_ps(A,B,0x71);
					_mm_store_ss(&ret,R);
					return ret;
			
				#else
					#warning "Using SSE3 dot product"
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
			
			
				float tmp[4];
				float ret;
				
				tmp[0]=a.x*b.x;
				tmp[1]=a.y*b.y;
				tmp[2]=a.z*b.z;
				tmp[3]=a.w*b.w;
				
				ret = tmp[0]+tmp[1]+tmp[2]+tmp[3];
				return ret;
			#endif
			}
			
			Vector operator^(Vector &a,Vector &b)
			{
				Vector r;
				
				r.x = a.y*b.z - a.z*b.y;
				r.y = a.z*b.x - a.x*b.z;
				r.z = a.x*b.y - a.y*b.x;
				r.w=0.0f;
				
				return r;
						
			}
			
			Vector operator*(Vector &a,float s)
			{
			#ifdef _IRIS_SSE_
				Vector r;
			
				__m128 V;
				__m128 F;
				__m128 R;
				V=_mm_loadu_ps(a.data);
				F=_mm_set1_ps(s);
				R=_mm_mul_ps(V,F);
				_mm_storeu_ps(r.data,R);
			
				return r;
			#else
			
				Vector r(a.x*s,a.y*s,a.z*s,a.w*s);
			
				return r;
			#endif
			}
			
			Vector operator*(Vector &v,Matrix &m)
			{
				Vector r;
				
				r.x=v.x*m.data[0] + v.y*m.data[4] + v.z*m.data[8] + v.w*m.data[12];
				r.y=v.x*m.data[1] + v.y*m.data[5] + v.z*m.data[9] + v.w*m.data[13];
				r.z=v.x*m.data[2] + v.y*m.data[6] + v.z*m.data[10] + v.w*m.data[14];
				r.w=v.x*m.data[3] + v.y*m.data[7] + v.z*m.data[11] + v.w*m.data[15];
						 
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
				
				r.x=a.x + factor*(b.x-a.x);
				r.y=a.y + factor*(b.y-a.y);
				r.x=a.z + factor*(b.z-a.z);
				r.x=a.w + factor*(b.w-a.w);
									
				return r;
			}
		}
	}
}

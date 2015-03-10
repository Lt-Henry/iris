
#include "Mesh.hpp"

#include <iostream>
#include <cmath>

using namespace std;
using namespace com::toxiclabs::iris;


bool Triangle::RayCollision(Vector & origin,Vector & direction,Vector & collision)
{
	Vector vab;
	Vector vac;
	Vector col;
	Vector Nv;
	
	
	float cosAlpha;
	float D;
	float deltaD;
	float t;
	
	
	
	int i0,i1,i2;
	float u0,u1,u2,v0,v1,v2;

	float alpha,beta;
	
	Nv = pnormal;
	
	bool ret=false;
	 
	
	
	//D=A*normal
	D = vertices[0] * Nv;
	
	//incident angle
	cosAlpha = direction * Nv;
		
	// deltaD = D - origin*normal;
	deltaD = D - (origin * Nv);
	
	//t=(deltaD/cosAlpha);
	t=deltaD/cosAlpha;
	
	//plane collision test
	if(t>=0.0f)
	{
		//col = origin + direction*t;
		col = (direction * t) + origin;
		
		//array<Vector,3> vertices=this->vertices;	
		/*
		Nv[0]=fabs(trNormal[0]);
		Nv[1]=fabs(trNormal[1]);
		Nv[2]=fabs(trNormal[2]);
		*/
		
		Nv.Abs();
		
		/*
		if(Nv.v[0]>max(Nv.v[1],Nv.v[2])){i0=0;i1=1;i2=2;}
		if(Nv.v[1]>max(Nv.v[0],Nv.v[2])){i0=1;i1=0;i2=2;}
		if(Nv.v[2]>max(Nv.v[0],Nv.v[1])){i0=2;i1=0;i2=1;}
		*/
	
		/*
		if(Nv[0]>Maxf(Nv[1],Nv[2]))
		{
			i0=0;
			i1=1;
			i2=2;
		}
		else
		{
		
			if(Nv[1]>Maxf(Nv[0],Nv[2]))
			{
				i0=1;
				i1=0;
				i2=2;
			}
			else
			{
				i0=2;
				i1=0;
				i2=1;
			}
		}
		*/
		
		int c1 = Nv[0]>Maxf(Nv[1],Nv[2]);
		int c2 = Nv[1]>Maxf(Nv[0],Nv[2]);
		
		i0 = 2 - (2*c1) - c2;
		i1 = c1;
		i2 = 3 - i0 - i1;
		
		u0=col[i1] - vertices[0][i1];
		v0=col[i2] - vertices[0][i2];
		
		u1=vertices[1][i1] - vertices[0][i1];
		u2=vertices[2][i1] - vertices[0][i1];
		
		v1=vertices[1][i2] - vertices[0][i2];
		v2=vertices[2][i2] - vertices[0][i2];
		
		if(AproxToZero(u1))
		{
			beta = u0/u2;
			alpha = (v0-beta*v2)/v1;
			
			
		}
		else
		{
			beta = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
			alpha = (u0 - beta*u2)/u1;
			
			
		}
		
		ret = (beta>=0.0f && beta<=1.0f) && (alpha>=0.0f && (alpha+beta)<=1.0f);
		collision=col;
	}
	
	return ret;
}

void Triangle::Print()
{
	for(int n=0;n<3;n++)
		cout<<"["<<vertices[n].x<<","<<vertices[n].y<<","<<vertices[n].z<<"] ";
	
	cout<<endl;	
}

Vector Triangle::GetCentroid()
{
	Vector ret;
	
	ret = vertices[0] + vertices[1] + vertices[2];
	
	ret = ret * 0.3333f;
	
	return ret; 
}

BoundBox Triangle::GetBoundBox()
{

	BoundBox ret;
	
	ret.min=vertices[0];
	ret.max=vertices[0];

	for(int n=0;n<3;n++)
	{
		if(vertices[n].x>ret.max.x)ret.max.x=vertices[n].x;
		if(vertices[n].y>ret.max.y)ret.max.y=vertices[n].y;
		if(vertices[n].z>ret.max.z)ret.max.z=vertices[n].z;
		
		if(vertices[n].x<ret.min.x)ret.min.x=vertices[n].x;
		if(vertices[n].y<ret.min.y)ret.min.y=vertices[n].y;
		if(vertices[n].z<ret.min.z)ret.min.z=vertices[n].z;
	}
	
	return ret;
}

BoundBox com::toxiclabs::iris::operator+(BoundBox & a,BoundBox & b)
{
	BoundBox ret;
	
	ret.min.x = (a.min.x<b.min.x) ? a.min.x : b.min.x;
	ret.min.y = (a.min.y<b.min.y) ? a.min.y : b.min.y;
	ret.min.z = (a.min.z<b.min.z) ? a.min.z : b.min.z;
	
	ret.max.x = (a.max.x>b.max.x) ? a.max.x : b.max.x;
	ret.max.y = (a.max.y>b.max.y) ? a.max.y : b.max.y;
	ret.max.z = (a.max.z>b.max.z) ? a.max.z : b.max.z;
		
	return ret;
}


Vector Triangle::PerturbateNormal(float angle,float r1,float r2)
{
	Vector trNormal;
	Vector R1;
	Vector R2;
	Vector pN;


	float factor1 = M_PI * 2.0f * r1;
	float factor2 =  angle*std::sqrt(r2);

	/*
	 I need to pick a normal
	 which one is the best?
	*/
	trNormal=normals[0];
	
	
	float x = std::cos( factor1 ) * factor2;
	float y = std::sin( factor1 ) * factor2;
	float z = std::sqrt( 1.0f - (factor2 * factor2) );
	
	R1 = vertices[0] - vertices[1];
	R2 = R1 ^ trNormal;
	
	R1.Normalize();
	R2.Normalize();	
	
	R1 = R1 * x;
	R2 = R2 * y;
	trNormal = trNormal * z;
	
	pN = R1 + R2 + trNormal;
	pN.Normalize();
	
	return pN;	
}


Vector Triangle::GetUVW(Vector & collision)
{

	Vector r;
	
	Vector f1;
	Vector f2;
	Vector f3;
	
	Vector p12;
	Vector p13;
	
	Vector tmp;
	
	float a,a1,a2,a3;
	
	f1 = vertices[0] - collision;//AP
	f2 = vertices[1] - collision;//BC
	f3 = vertices[2] - collision;//CP
	
	p12 = vertices[0] - vertices[1];//AB
	p13 = vertices[0] - vertices[2];//AC
	
	tmp = p12 ^ p13;
	a=tmp.Module();
	
	tmp = f2 ^ f3;
	a1=tmp.Module()/a;
	
	tmp = f3 ^ f1;
	a2=tmp.Module()/a;
	
	tmp = f1 ^ f2;
	a3=tmp.Module()/a;
	
	r.x=a1;
	r.y=a2;
	r.z=a3;
	r.w=1.0f;
		
	return r;
}


Vector Triangle::GetAveragedNormal(Vector & collision)
{
	Vector uvw = GetUVW(collision);
	
	Vector normal;
	Vector n1;
	Vector n2;
	Vector n3;
	
	n1 = normals[0] * uvw.data[0];
	n2 = normals[1] * uvw.data[1];
	n3 = normals[2] * uvw.data[2];
	normal = n1 + n2 + n3;
	normal.Normalize();	

	return normal;	
}

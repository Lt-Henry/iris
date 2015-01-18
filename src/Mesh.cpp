
#include "Mesh.hpp"

#include <iostream>

using namespace std;
using namespace com::toxiclabs::iris;


bool Triangle::RayCollision(Vector & origin,Vector & direction,Vector & collision)
{
	Vector vab;
	Vector vac;
	
	float cosAlpha;
	
	float D;
	float deltaD;
	float t;
	
	Vector col;
	Vector Nv;
	
	int i0,i1,i2;
	float u0,u1,u2,v0,v1,v2;

	float alpha,beta;
	
	Nv = pnormal;
	
	bool ret=false;
	 
	
	//first check
	
	cosAlpha = direction * Nv;
	
	//D=A*normal
	D = vertices[0] * Nv;
	
	
	// deltaD = D - origin*normal;
	deltaD = D - (origin * Nv);
	
	//t=(deltaD/cosAlpha);
	t=deltaD/cosAlpha;
	
	//plane collision test
	if(t>=0.0f)
	{
		//col = origin + direction*t;
		col = (direction * t) + origin;
		
			
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

BoundBox operator+(BoundBox & a,BoundBox & b)
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

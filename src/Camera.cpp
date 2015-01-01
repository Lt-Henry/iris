
#include "Camera.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace com::toxiclabs::iris;


Camera::Camera()
{

	position.Set(0.0f,0.0f,0.0f,1.0f);
	target.Set(0.0f,0.0f,5.0f,1.0f);
	
	angle=45.0f;
	near=5.0f;
	far=100.0f;
	
	name="default";
	
	CreateMatrix();
}

Camera::Camera
(string name,Vector & position,Vector & target,
float angle,float near,float far)
{
	this->name=name;
	this->position=position;
	this->target=target;
	this->angle=angle;
	this->near=near;
	this->far=far;	
	

	CreateMatrix();
}

Camera::~Camera()
{
}

void Camera::CreateMatrix()
{
	Vector forward;
	Vector up;
	Vector right;
	
	
	
	forward = target - position;
	forward.Normalize();
	forward.Negate();
	
	
	up.x=std::sin(DegToRad(angle));
	up.y=std::cos(DegToRad(angle));
	up.z=0.0f;
	up.w=0.0f;
	
	right = forward ^ up;
	right.Normalize();
	
	Matrix mT;
	Matrix mR;
	
	mT = Matrix::CreateTranslation(-position.x,-position.y,-position.z);
		
	
	up = right ^ forward;	
	up.Normalize();
	
	mR = Matrix::CreateIdentity();
	
	//right
	mR.data[0]=right.x;
	mR.data[4]=right.y;
	mR.data[8]=right.z;
	mR.data[12]=0.0f;
	
	//up
	mR.data[1]=up.x;
	mR.data[5]=up.y;
	mR.data[9]=up.z;
	mR.data[13]=0.0f;
	
	//forward
	mR.data[2]=-forward.x;
	mR.data[6]=-forward.y;
	mR.data[10]=-forward.z;
	mR.data[14]=0.0f;
	
	mR.data[3]=0.0f;
	mR.data[7]=0.0f;
	mR.data[11]=0.0f;
	mR.data[15]=1.0f;
	
		
	matrix = mR*mT;
}

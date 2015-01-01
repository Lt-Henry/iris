
#include "Color.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


ColorType Color::GetType()
{
	return type;
}


ColorRGB::ColorRGB()
{
	type=ColorType::RGB;
}

ColorRGB::ColorRGB(float r,float g,float b,float a) : ColorRGB()
{
	this->r=r;
	this->g=g;
	this->b=b;
	this->a=a;
}

void ColorRGB::Black()
{
	r=0.0f;
	g=0.0f;
	b=0.0f;
	a=1.0f;
}

void ColorRGB::White()
{
	r=1.0f;
	g=1.0f;
	b=1.0f;
	a=1.0f;
}

void ColorRGB::Clamp()
{
	for(int n=0;n<4;n++)
	{
		if(data[n]>1.0f)data[n]=1.0f;
		if(data[n]<0.0f)data[n]=0.0f;
	}
	
}

uint32_t ColorRGB::ToPixel()
{
	uint32_t pixel = 0;
	
	uint32_t cr;
	uint32_t cg;
	uint32_t cb;
	uint32_t ca;
	
	cr = data[0]*255;
	cg = data[1]*255;
	cb = data[2]*255;
	ca = data[3]*255;
	
	pixel = ca + (cb<<8) + (cg<<16) + (cr<<24); 
	
	return pixel;
}


ColorXYZ::ColorXYZ()
{
	type=ColorType::XYZ;
}


ColorXYZ::ColorXYZ(float x,float y,float z) : ColorXYZ()
{
	this->x=x;
	this->y=y;
	this->z=z;
}

void ColorXYZ::Black()
{
	x=0.0f;
	y=0.0f;
	z=0.0f;	
}

void ColorXYZ::White()
{
	x=1.0f;
	y=1.0f;
	z=1.0f;
}

void ColorXYZ::Clamp()
{
	for(int n=0;n<4;n++)
	{
		if(data[n]>1.0f)data[n]=1.0f;
		if(data[n]<0.0f)data[n]=0.0f;
	}
}


ColorRGB ColorXYZ::ToRGB()
{
	ColorRGB ret;
		
	ret.r=3.240479f*x - 1.537150f*y - 0.498535f*z;
	ret.g=-0.969256f*x + 1.875991f*y + 0.041556f*z;
	ret.b=0.055648f*x - 0.204043f*y + 1.057311f*z;
	ret.a=1.0f;
	
	ret.Clamp();
	return ret;
}


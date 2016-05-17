
#include "Color.hpp"
#include "Math.hpp"

#include <iostream>

using namespace Color;
using namespace std;


XYZ::XYZ()
{
	type=Type::XYZ;
}


XYZ::XYZ(float x,float y,float z) : XYZ()
{
	this->x=x;
	this->y=y;
	this->z=z;
}

void XYZ::black()
{
	x=0.0f;
	y=0.0f;
	z=0.0f;	
}

void XYZ::white()
{
	x=1.0f;
	y=1.0f;
	z=1.0f;
}

void XYZ::clamp()
{
	for(int n=0;n<4;n++)
	{
		if(data[n]>1.0f)data[n]=1.0f;
		if(data[n]<0.0f)data[n]=0.0f;
	}
}


HSV::HSV()
{
	type=Type::HSV;
}


HSV::HSV(float h,float s,float v):HSV()
{
	this->h=h;
	this->s=s;
	this->v=v;
}

void HSV::white()
{
	h=0.0f;
	s=0.0f;
	v=1.0f;
}

void HSV::black()
{
	h=0.0f;
	s=1.0f;
	v=0.0f;
}

void HSV::clamp()
{
	if (h<0.0f) {
		h=0.0f;
	}
	
	if (s<0.0f) {
		s=0.0f;
	}
	
	if (v<0.0f) {
		v=0.0f;
	}
	
	if (h>360.0f) {
		h=360.0f;
	}
	
	if (s>1.0f) {
		s=1.0f;
	}
	
	if (v>1.0f) {
		v=1.0f;
	}
}



RGB::RGB()
{
	type=Type::RGB;
}

RGB::RGB(float r,float g,float b,float a) : RGB()
{
	this->r=r;
	this->g=g;
	this->b=b;
	this->a=a;
}

void RGB::black()
{
	r=0.0f;
	g=0.0f;
	b=0.0f;
	a=1.0f;
}

void RGB::white()
{
	r=1.0f;
	g=1.0f;
	b=1.0f;
	a=1.0f;
}

void RGB::clamp()
{
	for (int n=0;n<4;n++) {
		if (data[n]>1.0f) data[n]=1.0f;
		if (data[n]<0.0f) data[n]=0.0f;
	}
	
}

void RGB::clamp(float min,float max)
{
	for (int n=0;n<4;n++) {
		if (data[n]>max) data[n]=max;
		if (data[n]<min) data[n]=min;
	}
}

uint32_t RGB::pixel()
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

RGB Color::operator*(ColorRGB c,float f)
{
	RGB r;
	
	for (int n=0;n<4;n++) {
		r.data[n]=c.data[n]*f;
	}
	
	return r;
}

RGB Color::operator/(ColorRGB c,float f)
{
	RGB r;
	
	for (int n=0;n<4;n++) {
		r.data[n]=c.data[n]/f;
	}
	
	return r;
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


ColorRGB ColorHSV::ToRGB()
{
	ColorRGB color;

	int hi;
	float hp;

	float f;
	float pv,qv,tv;

	if(IsZero(s))
	{
		color.r=v;
		color.g=v;
		color.b=v;
	}
	else
	{
		hp=h/60.0f;
		hi=std::floor(hp);

		f=hp-hi;

		pv = v * ( 1 - s );
		qv = v * ( 1 - s * f );
		tv = v * ( 1 - s * ( 1 - f ) );

		switch(hi)
		{
			case 0:
				color.r=v;
				color.g=tv;
				color.b=pv;
			break;

			case 1:
				color.r=qv;
				color.g=v;
				color.b=pv;
			break;

			case 2:
				color.r=pv;
				color.g=v;
				color.b=tv;
			break;

			case 3:
				color.r=pv;
				color.g=qv;
				color.b=v;
			break;

			case 4:
				color.r=tv;
				color.g=pv;
				color.b=v;
			break;

			case 5:
				color.r=v;
				color.g=pv;
				color.b=qv;
			break;
		}

	}


	return color;
}

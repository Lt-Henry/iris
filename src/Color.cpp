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

#include "Color.hpp"
#include "Math.hpp"

#include <iostream>

using namespace Color;
using namespace Math;

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


RGB::RGB(XYZ & xyz) : RGB()
{
		
	r=3.240479f*xyz.x - 1.537150f*xyz.y - 0.498535f*xyz.z;
	g=-0.969256f*xyz.x + 1.875991f*xyz.y + 0.041556f*xyz.z;
	b=0.055648f*xyz.x - 0.204043f*xyz.y + 1.057311f*xyz.z;
	a=1.0f;
	
	Clamp();
}



RGB::RGB(HSV & hsv) : RGB()
{
	

	int hi;
	float hp;

	float f;
	float pv,qv,tv;

	a=1.0f;

	if (is_zero(s)) {
		r=hsv.v;
		g=hsv.v;
		b=hsv.v;
	}
	else {
		hp=hsv.h/60.0f;
		hi=std::floor(hp);

		f=hp-hi;

		pv = hsv.v * ( 1 - hsv.s );
		qv = hsv.v * ( 1 - hsv.s * f );
		tv = hsv.v * ( 1 - hsv.s * ( 1 - f ) );

		switch (hi) {

			case 0:
				r=hsv.v;
				g=tv;
				b=pv;
			break;

			case 1:
				r=qv;
				g=hsv.v;
				b=pv;
			break;

			case 2:
				r=pv;
				g=hsv.v;
				b=tv;
			break;

			case 3:
				r=pv;
				g=qv;
				b=hsv.v;
			break;

			case 4:
				r=tv;
				g=pv;
				b=hsv.v;
			break;

			case 5:
				r=hsv.v;
				g=pv;
				b=qv;
			break;
		}

	}


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



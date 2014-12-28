
#include "Color.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


void Color::Clear()
{
	data[0]=0.0f;
	data[1]=0.0f;
	data[2]=0.0f;
	data[3]=0.0f;
}


void Color::Clamp()
{
	for(int n=0;n<4;n++)
	{
		if(data[n]>1.0f)data[n]=1.0f;
		if(data[n]<0.0f)data[n]=0.0f;
	}
}

uint32_t Color::ToPixelRGBA()
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

#include "Sensor.hpp"


using namespace std;
using namespace com::toxiclabs::iris;


Sensor::Sensor(int width,int height,float sensor_width,float sensor_height)
{
	this->width=width;
	this->height=height;
	this->sensor_width=sensor_width;
	this->sensor_height=sensor_height;

	buffer=new float[width*height];

}


Sensor::~Sensor()
{
	delete buffer;
}

int Sensor::GetWidth()
{
	return width;
}

int Sensor::GetHeight()
{
	return height;
}

void Sensor::SetPixel(int x,int y,Spectrum & spr)
{

	unsigned char value;

	value=(x%2) | ((y%2)<<1);

	float value=0.0f;

	//Bayer filter RGGB
	switch(value)
	{
		//R
		case 0:
			ColorRGB
		break;

		//G
		case 1:
		case 2:

		break;

		//B
		case 3:

		break;
	}

}

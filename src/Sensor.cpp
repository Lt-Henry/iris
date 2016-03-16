#include "Sensor.hpp"


using namespace std;
using namespace com::toxiclabs::iris;


Sensor::Sensor(Settings & settings)
{
	width=settings.Get("sensor.width",800);
	height=settings.Get("sensor.height",600);

	sensor_width=settings.Get("sensor.size.width",40.0f);
	sensor_height=settings.Get("sensor.size.height",30.0f);

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


	//Bayer filter RGGB
	switch(value)
	{
		//R
		case 0:

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

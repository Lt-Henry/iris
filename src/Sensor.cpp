#include "Sensor.hpp"


using namespace std;
using namespace com::toxiclabs::iris;


Sensor::Sensor(Settings & settings)
{
	cols=settings.Get("sensor.columns",800);
	rows=settings.Get("sensor.rows",600);

	width=settings.Get("sensor.width",0.04f);
	height=settings.Get("sensor.height",0.03f);

	buffer=new float[cols*rows];

	bitmap=new BitMap(cols,rows);
}


Sensor::~Sensor()
{
	delete buffer;
	delete bitmap;
}

float Sensor::GetWidth()
{
	return width;
}

float Sensor::GetHeight()
{
	return height;
}

int Sensor::GetColumns()
{
	return cols;
}

int Sensor::GetRows()
{
	return rows;
}

void Sensor::SetCell(int x,int y,Spectrum & spr)
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


BitMap * Sensor::Process()
{
	//generate random noise
	
	for(int r=0;r<rows;r++)
	{
		for(int c=0;c<cols;c++)
		{
			float f=rand()/(float)RAND_MAX;
			buffer[c+r*cols]=f;
			
			ColorRGB color;
			
			color.r=f;
			color.g=f;
			color.b=f;
			color.a=1;
			bitmap->PutPixel(c,r,color);
		}
	}

	return bitmap;
}

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

void Sensor::Clear()
{
	for(int r=0;r<rows;r++)
	{
		for(int c=0;c<cols;c++)
		{
			buffer[c+r*cols]=0.0f;
		}
	}
}

BitMap * Sensor::Process()
{

	//clear sensor
	Clear();

	//generate random noise
	float noise=0.3f;
	
	for(int r=0;r<rows;r++)
	{
		for(int c=0;c<cols;c++)
		{
			float f=rand()/(float)RAND_MAX;
			
			float v = buffer[c+r*cols];
			
			buffer[c+r*cols]= v + (noise*f);
		}
	}
	
	//ADC
	for(int r=0;r<rows;r++)
	{
		for(int c=0;c<cols;c++)
		{
			float v = buffer[c+r*cols];
			
			ColorRGB color;
			
			unsigned char value;

			value=(c%2) + ((r%2)*2);
			

			//Bayer filter RGGB
			switch(value)
			{
				//R
				case 0:
					color.r=v;
					color.g=0;
					color.b=0;
				break;

				//G
				case 1:
				case 2:
					color.r=0;
					color.g=v;
					color.b=0;
				break;

				//B
				case 3:
					color.r=0;
					color.g=0;
					color.b=v;
				break;
			}


			color.a=1;
			bitmap->PutPixel(c,r,color);
		}
	}

	return bitmap;
}

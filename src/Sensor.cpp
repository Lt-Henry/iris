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
	float noise=0.1f;
	
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
			//simple debayer
			
			int nc;
			int nr;
			
			if(r==rows-1)
			{
				nr=-1;
			}
			else
			{
				nr=1;
			}
			
			if(c==cols-1)
			{
				nc=-1;
			}
			else
			{
				nc=1;
			}

			
			float A,B,C,D;
			ColorRGB color;
			unsigned char value;

			value=(c%2) + ((r%2)*2);

			/*
				AB
				CD
			*/
			A=buffer[c+r*cols];
			B=buffer[(c+nc)+r*cols];
			C=buffer[c+(r+nr)*cols];
			D=buffer[(c+nc)+(r+nr)*cols];


			//Bayer filter RGGB
			switch(value)
			{
				/*
					RG
					GB
				*/
				case 0:
					color.r=A;
					color.g=(B*0.5f)+(C*0.5f);
					color.b=D;
				break;

				/*
					GR
					BG
				*/
				case 1:
					color.r=B;
					color.g=(A*0.75f)+(D*0.25f);
					color.b=C;				
				break;
				
				/*
					GB
					RG
				*/
				case 2:
					color.r=C;
					color.g=(A*0.75f)+(D*0.25f);
					color.b=B;
				break;

				/*
					BG
					GR
				*/
				case 3:
					color.r=D;
					color.g=(B*0.5f)+(C*0.5f);
					color.b=A;
				break;
			}


			color.a=1;
			bitmap->PutPixel(c,r,color);
		}
	}

	return bitmap;
}

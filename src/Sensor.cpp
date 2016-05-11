#include "Sensor.hpp"
#include "Math.hpp"

using namespace std;
using namespace com::toxiclabs::iris;


Sensor::Sensor(Settings & settings)
{
	cols=settings.Get("sensor.columns",800);
	rows=settings.Get("sensor.rows",600);

	width=settings.Get("sensor.width",0.04f);
	height=settings.Get("sensor.height",0.03f);
	
	exposure_time=settings.Get("sensor.exposure_time",0.25f);
	sensivity=settings.Get("sensor.sensivity",625.0f);
	dark_current=settings.Get("sensor.dark_current",16.0f);
	dynamic_range=settings.Get("sensor.dynamic_range",46.0f);

	
	buffer=new float[cols*rows];

	bitmap=new BitMap(cols,rows);
	
	//create fake bayer filters
	for(int n=0;n<32;n++)
	{
		red.data[n]=Spectrum::X[n];
		green.data[n]=Spectrum::Y[n];
		blue.data[n]=Spectrum::Z[n];
	}
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
	
	Spectrum s;
	

	value=(x%2) + ((y%2)*2);


	//Bayer filter RGGB
	switch(value)
	{
		//R
		case 0:
			s=spr*red;
		break;

		//G
		case 1:
		case 2:
			s=spr*green;
		break;

		//B
		case 3:
			s=spr*blue;
		break;
	}

	int index=x+y*cols;
	
	buffer[index]=s.Energy();

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
	float noise=dark_current*exposure_time;
	
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

			float dr=Linear(dynamic_range);

			//saturation
			color.Clamp(0.0f,dr);
			
			//normalize
			color=color/dr;

			color.a=1;
			bitmap->PutPixel(c,r,color);
		}
	}

	return bitmap;
}

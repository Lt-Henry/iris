
#include "Settings.hpp"
#include "Atmosphere.hpp"
#include "BitMap.hpp"
#include "Color.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


int main(int argc,char * argv[])
{

	cout<<"Iris Test"<<endl;
	
	Settings settings;

	Atmosphere atmosphere(settings);
	
	BitMap bitmap(640,480);
	
	for(int i=0;i<640;i++)
	{
		for(int j=0;j<480;j++)
		{
			ColorRGB color;
			
			color.r=0;
			color.g=i/640.0f;
			color.b=j/480.0f;
			color.a=1.0f;
			
			bitmap.PutPixel(i,j,color);
		}
	}
	
	bitmap.Save("out.png");

	return 0;
}

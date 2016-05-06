
#include "Settings.hpp"
#include "Atmosphere.hpp"
#include "Sensor.hpp"
#include "Color.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


int main(int argc,char * argv[])
{

	cout<<"Iris Test"<<endl;
	
	Settings settings;

	Atmosphere atmosphere(settings);
	/*
	Sensor sensor(settings);
	
	BitMap * bitmap;
	
	bitmap=sensor.Process();
	bitmap->Save("out.png");
	*/
	return 0;
}

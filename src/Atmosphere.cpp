
#include "Atmosphere.hpp"

#include <cmath>

using namespace std;
using namespace com::toxiclabs::iris;


Atmosphere::Atmosphere()
{
	sun_position=Vector(0.5,0.8,0.5,0.0);
	sun_position.Normalize();
	
	sunlight=Spectrum(4500);
	sky=Spectrum(9000);
	
	sunlight.Normalize();
	sunlight=sunlight*3.0f;
	
	sky.Normalize();
	
}


Spectrum Atmosphere::Energy(Vector & direction)
{
	float factor;
	Spectrum energy;
	
	factor = direction * sun_position;
	
	if(factor<0.0f)
	{
		factor=0.0f;
	}
	
	energy=(sunlight*std::pow(factor,4));
	energy=(sky*(1.0f-factor)) + energy;
	
	return energy;
}


#include "Atmosphere.hpp"

#include <cmath>

using namespace std;
using namespace com::toxiclabs::iris;


Atmosphere::Atmosphere(Settings & settings)
{
	int year;
	int month;
	int day;

	int hour;
	int minute;

	float latitude;
	float longitude;


	//get some vars
	year=settings.Get("world.year",2016);
	month=settings.Get("world.month",1);
	day=settings.Get("world.day",1);

	hour=settings.Get("world.hour",12);
	minute=settings.Get("world.minute",0);

	latitude=settings.Get("world.latitude",39.471690f);
	longitude=settings.Get("world.longitude",-0.323460f);


	//compute sun position


	int jd;

	//get julian date
	jd=367*year - int(7*(year+int((month+9)/12))/4);

	cout<<"Julian date:"<<jd<<endl;

	sun_position=Vector(0.0,1.0,1.0,0.0);
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
	

	energy=(sunlight*std::pow(factor,80));
	energy=energy + sky;
	//energy=(sky*(1.0f-factor)) + energy;
	
	return energy;
}

void Atmosphere::Mult(Matrix & m)
{
	sun_position = sun_position * m;
}

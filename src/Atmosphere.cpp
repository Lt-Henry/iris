
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


	float jd;
	float A,B;
	
	if(month<=2)
	{
		month+=12;
		year-=1;
	}
	
	A=int(year/100.0f);
	B=2-A+int(A/4.0f);
	
	
	
	//get julian date
	
	jd=int(365.25f * (year+4716.0f)) + int(30.6001f * (month+1)) + day + B - 1524.5f;

	cout<<"Julian date:"<<int(jd+0.5f)<<endl;
	
	//ecliptic coordinates
	float n;
	float L;
	float g;
	float lambda;
	float beta;
	float R;
	
	//number of days since Greenwhich noon
	n = jd - 2451545.0f;
	
	//mean latitude
	L = 280.460f + 0.9856474f * n;
	
	//mean anomaly
	g = 357.528f + 0.9856003f * n;
	
	//ecliptic longitude
	lambda = L + 1.915f * sin(g) + 0.020f * sin(2.0f*g);
	
	//ecliptic latitude
	beta = 0.0f;
	
	//Sun distance in astronomical units
	R = 1.00014f - 0.01671f *cos(g) - 0.00014f*cos(2.0f*g);
	
	//equatorial coordinates
	float ascension;
	float declination;
	
	ascension=asin(sin(-23.44f) * sin(lambda));
	
	cout<<"Longitude: "<<lambda<<endl;
	cout<<"ascension: "<<ascension<<endl;

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

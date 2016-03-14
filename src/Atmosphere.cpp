
#include "Atmosphere.hpp"

#include <cmath>
#include <iomanip>

using namespace std;
using namespace com::toxiclabs::iris;

const float Atmosphere::obliquity=0.409f; // 23.44 degrees;


Atmosphere::Atmosphere(Settings & settings)
{
	int year;
	int month;
	int day;

	int hour;
	int minute;

	float latitude;
	float longitude;
	float elevation;


	//get some vars
	year=settings.Get("world.year",2016);
	month=settings.Get("world.month",1);
	day=settings.Get("world.day",1);

	hour=settings.Get("world.hour",12);
	minute=settings.Get("world.minute",0);

	latitude=settings.Get("world.latitude",39.471690f);
	longitude=settings.Get("world.longitude",-0.323460f);
	elevation=settings.Get("world.elevation",0.0f);


	/*
	 * compute sun position
	 * 
	 * Expected altitude
	 * 27.527778022354987
	 * 
	 * Expected Azimuth 
	 * -358.8058486943063
	 */

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

	cout<<std::fixed<<std::setprecision(3)<<"Julian date:"<<jd<<endl;
	
	//ecliptic coordinates
	float n;
	float gmls; //geometric mean longitude of sun
	float gmas; //geometric mean anomaly of sun
	float eLon;
	float eLat;
	float R;
	
	//number of days since Greenwhich noon
	n = jd - 2451545.0f;
	
	//geometric mean longitude of sun
	gmls = DegToRad(280.460f) + (DegToRad(0.9856474f) * n);
	
	//geometric mean anomaly of sun
	gmas = DegToRad(357.528f) + (DegToRad(0.9856003f) * n);
	
	//ecliptic longitude
	eLon = gmls + (DegToRad(1.915f) * sin(gmas)) + (DegToRad(0.020f) * sin(2.0f*gmas));
	
	//ecliptic latitude
	eLat = 0.0f;
	
	//Sun distance in astronomical units
	R = 1.00014f - 0.01671f *cos(gmas) - 0.00014f*cos(2.0f*gmas);
	
	//equatorial coordinates
	float ascension;
	float declination;
	
	ascension=atan(cos(-Atmosphere::obliquity) * tan(eLon));
	declination=asin(sin(-Atmosphere::obliquity) * sin(eLon));
	
	cout<<"days since Greenwhich noon: "<<n<<endl;
	cout<<"geometric mean longitude: "<<RadToDegNice(gmls)<<endl;
	cout<<"geometric mean anomaly: "<<RadToDeg(gmas)<<endl;
	cout<<"ecliptic longitude: "<<RadToDegNice(eLon)<<endl;
	cout<<"ecliptic latitude: "<<RadToDegNice(eLat)<<endl;
	cout<<"sun distance: "<<R<<endl;
	cout<<"ascension: "<<RadToDegNice(ascension)<<endl;
	cout<<"declination: "<<RadToDegNice(declination)<<endl;

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

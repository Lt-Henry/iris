
#include "Atmosphere.hpp"

#include <cmath>
#include <iomanip>

using namespace std;
using namespace com::toxiclabs::iris;



Atmosphere::Atmosphere(Settings & settings)
{
	int year;
	int month;
	int day;

	int hour;
	int minute;

	int timezone;

	float latitude;
	float longitude;
	float elevation;


	//get some vars
	year=settings.Get("world.year",2016);
	month=settings.Get("world.month",1);
	day=settings.Get("world.day",1);

	hour=settings.Get("world.hour",0);
	minute=settings.Get("world.minute",6);

	timezone=settings.Get("world.timezone",0);

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

	//julian day
	float jd;
	//julian century
	float jc;


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

	//add time to julian date
	jd=jd+((hour-timezone)/24.0)+(minute/(24.0f*60.0f));

	jc=(jd - 2451545.0f) / 36525.0f;

	cout<<std::fixed<<std::setprecision(4)<<"Julian date:"<<jd<<endl;
	cout<<"Julian century:"<<jc<<endl;


	float gmls; //geometric mean longitude of sun
	float gmas; //geometric mean anomaly of sun
	float eeo; //eccent earth orbit
	float seoc; //sun equation of center
	float stl; //sun true longitude
	float sta; //sun true anomaly
	float srv; //sun radian vector
	float sal; //sun apparent longitude
	float moe; //mean oblique ecliptic
	float oc; //oblique correction
	float sra; //sun right ascension
	float sd; //sun declination



	//geometric mean longitude of sun
	gmls = DegToRad(280.46646f) + (jc * (DegToRad(36000.76983f)+ jc*DegToRad(0.0003032f)));

	//geometric mean anomaly of sun
	gmas = DegToRad(357.52911f) + jc * (DegToRad(35999.05029f)-DegToRad(0.0001537f)*jc);

	//eccent earth orbit
	eeo=0.016708634f-jc*(4.2037e-05f+1.267e-07f*jc);

	//sun equation of center
	//constants in degrees
	seoc=sin(gmas)*(1.914602f-jc*(0.004817f+1.4e-05f*jc))+sin((2.0f*gmas))*(0.019993f-0.000101f*jc)+sin((3.0f*gmas))*0.000289f;
	seoc=DegToRad(seoc);


	//sun true longitude
	stl=gmls+seoc;

	//sun true anomaly
	sta=gmas+seoc;

	//sun radian vector (in astronomical units)
	srv=(1.000001018f*(1.0f-eeo*eeo))/(1.0f+eeo*cos(sta));


	//sun apparent longitude
	sal=stl-DegToRad(0.00569f)-DegToRad(0.00478f)*sin(DegToRad(125.04f)-DegToRad(1934.136f)*jc);


	//mean oblique ecliptic (constants in degrees)
	moe=23.0f+(26.0f+((21.448f-jc*(46.815f+jc*(0.00059f-jc*0.001813f))))/60.0f)/60.0f;
	moe=DegToRad(moe);
	
	//oblique correction
	oc=moe+DegToRad(0.00256f)*cos(DegToRad(125.04f-1934.136f*jc));

	//sun right ascension
	sra=atan2(cos(oc)*sin(sal),cos(sal));

	//sun declination
	sd=asin(sin(oc)*sin(sal));
	//=degrees(asin(sin(radians(R2))*sin(radians(P2))))

	cout<<"geometric mean longitude: "<<RadToDegNice(gmls)<<endl;
	cout<<"geometric mean anomaly: "<<RadToDeg(gmas)<<endl;
	cout<<"eccent earth orbit: "<<eeo<<endl;
	cout<<"sun eq of ctr: "<<seoc<<endl;
	cout<<"sun true longitude: "<<RadToDegNice(stl)<<endl;
	cout<<"sun true anomaly: "<<RadToDeg(sta)<<endl;
	cout<<"sun radius vector: "<<srv<<endl;
	cout<<"sun apparent longitude: "<<RadToDegNice(sal)<<endl;
	cout<<"mean oblique ecliptic: "<<RadToDegNice(moe)<<endl;
	cout<<"oblique correction: "<<RadToDegNice(oc)<<endl;
	cout<<"sun right ascension: "<<RadToDeg(sra)<<endl;
	cout<<"sun declination: "<<RadToDeg(sd)<<endl;

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

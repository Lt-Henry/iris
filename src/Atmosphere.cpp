
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

	cout<<std::fixed<<std::setprecision(8)<<"Julian date: "<<jd<<endl;
	cout<<"Julian century: "<<jc<<endl;
	

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
	float vy; //var y
	float eot; //equation of time (minutes)
	float tst; //true solar time
	float ha; //hour angle
	float sza;	//solar zenith angle
	float sea; //solar elevation angle
	float aar; //approximate atmospheric refraction
	float secar; //solar elevation corrected for atmospheric refraction
	float saa; //solar azimuth angle


	//geometric mean longitude of sun
	gmls = Radians(280.46646f) + (jc * (Radians(36000.76983f)+ jc*Radians(0.0003032f)));
	

	//geometric mean anomaly of sun
	gmas = Radians(357.52911f) + jc * (Radians(35999.05029f)-Radians(0.0001537f)*jc);

	//eccent earth orbit
	eeo=0.016708634f-jc*(4.2037e-05f+1.267e-07f*jc);

	//sun equation of center
	//constants in degrees
	seoc=sin(gmas)*(1.914602f-jc*(0.004817f+1.4e-05f*jc))+sin((2.0f*gmas))*(0.019993f-0.000101f*jc)+sin((3.0f*gmas))*0.000289f;
	seoc=Radians(seoc);


	//sun true longitude
	stl=gmls+seoc;

	//sun true anomaly
	sta=gmas+seoc;

	//sun radian vector (in astronomical units)
	srv=(1.000001018f*(1.0f-eeo*eeo))/(1.0f+eeo*cos(sta));


	//sun apparent longitude
	sal=stl-Radians(0.00569f)-Radians(0.00478f)*sin(Radians(125.04f)-Radians(1934.136f)*jc);


	//mean oblique ecliptic (constants in degrees)
	moe=23.0f+(26.0f+((21.448f-jc*(46.815f+jc*(0.00059f-jc*0.001813f))))/60.0f)/60.0f;
	moe=Radians(moe);
	
	//oblique correction
	oc=moe+Radians(0.00256f)*cos(Radians(125.04f-1934.136f*jc));

	//sun right ascension
	sra=atan2(cos(oc)*sin(sal),cos(sal));

	//sun declination
	sd=asin(sin(oc)*sin(sal));
	
	//var y
	vy=tan(oc/2.0f)*tan(oc/2.0f);
	
	//equation of time
	eot=4.0f*(vy*sin(2.0f*(gmls))-2.0f*eeo*sin((gmas))+4.0f*eeo*vy*sin((gmas))*cos(2.0f*(gmls))-0.5f*vy*vy*sin(4.0f*(gmls))-1.25f*eeo*eeo*sin(2.0f*(gmas)));
	
	
	//true solar time
	tst=((hour*60.0f + minute)/1440.0f)*1440.0f+Degrees(eot)+4.0f*longitude-60.0f*timezone;
	
	//=if(AB2/4<0;AB2/4+180;AB2/4-180)
	ha=(tst/4.0f<0.0f) ? tst/4.0f+180.0f : tst/4.0f-180.0f;
	ha=Radians(ha);
	
	//solar zenith angle
	sza=acos(sin(Radians(latitude))*sin(sd)+cos(Radians(latitude))*cos(sd)*cos(ha));
	
	//solar elevation angle
	sea=(M_PI/2.0f)-sza;
	
	//approximate atmospheric refraction
	//mother of God :D
	if(Degrees(sea)>85.0)
	{
		aar=0.0f;
	}
	else
	{
		if(Degrees(sea)>5)
		{
			aar=58.1f/tan(sea)-0.07/std::pow(tan(sea),3)+8.6e-05/std::pow(tan(sea),5);
		}
		else
		{
			if(Degrees(sea)>-0.575f)
			{
				aar=1735+sea*(-518.2f+sea*(103.4f+sea*(-12.79f+sea*0.711f)));
			}
			else
			{
				aar=-20.772f/tan(sea);
			}
		}
	}
	
	aar=Radians(aar/3600.0f);
	
	//solar elevation corrected for atmospheric refraction
	secar=sea+aar;

	//solar azimuth angle
	if(Degrees(ha)>0)
	{
		saa=acos(((sin(Radians(latitude))*cos(sza))-sin(sd))/(cos(Radians(latitude))*sin(sza)))+M_PI;
	}
	else
	{
		saa=Radians(540.0f)-(acos(((sin(Radians(latitude))*cos(sza))-sin(sd))/(cos(Radians(latitude))*sin(sza))));
	}
	

	cout<<"geometric mean longitude: "<<DegreesNice(gmls)<<endl;
	cout<<"geometric mean anomaly: "<<Degrees(gmas)<<endl;
	cout<<"eccent earth orbit: "<<eeo<<endl;
	cout<<"sun eq of ctr: "<<Degrees(seoc)<<endl;
	cout<<"sun true longitude: "<<DegreesNice(stl)<<endl;
	cout<<"sun true anomaly: "<<Degrees(sta)<<endl;
	cout<<"sun radius vector: "<<srv<<endl;
	cout<<"sun apparent longitude: "<<DegreesNice(sal)<<endl;
	cout<<"mean oblique ecliptic: "<<DegreesNice(moe)<<endl;
	cout<<"oblique correction: "<<DegreesNice(oc)<<endl;
	cout<<"sun right ascension: "<<Degrees(sra)<<endl;
	cout<<"sun declination: "<<Degrees(sd)<<endl;
	cout<<"var y: "<<vy<<endl;
	cout<<"equation of time: "<<Degrees(eot)<<endl;
	cout<<"true solar time: "<<tst<<endl;
	cout<<"hour angle : "<<Degrees(ha)<<endl;
	cout<<"solar zenith angle: "<<Degrees(sza)<<endl;
	cout<<"solar elevation angle: "<<Degrees(sea)<<endl;
	cout<<"approximate atmospheric refaction: "<<Degrees(aar)<<endl;
	cout<<"solar elevation corrected for atmospheric refraction: "<<Degrees(secar)<<endl;
	cout<<"solar azimuth angle: "<<DegreesNice(saa)<<endl;


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

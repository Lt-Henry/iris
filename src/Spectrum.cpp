
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#include "Spectrum.hpp"

using namespace std;
using namespace com::toxiclabs::iris;

array<float,32> Spectrum::CIE_X = {
0.09564000f,0.06327000f,0.5945000f,1.0622000f, //420
0.4479000f,0.04677000f,0.002899327f,0.0001661505f,0.00001025398f, //470
0.0003699140f,0.03846824f,0.3390941f,0.03641283f,0.1538542f, //520
0.7455188f,1.0130477f,0.2980011f,0.02445275f,0.001546459f, //570
0.00008902390f,0.000005455118f,0.001094949f,0.1084063f,0.3032904f, //620
0.007967240f,0.2639922f,0.8878944f,0.8905020f,0.1851708f, //670
0.01294868f,0.0008007503f,0.00004767654f }; //700


array<float,32> Spectrum::CIE_Y = {
0.1390200f,0.7100000f,0.9950000f,0.6310000f, //420
0.1750000f,0.01700000f,0.001047000f,0.00006000000f,0.000003702900f, //470
0.00001104323f,0.001069880f,0.03622571f,0.1994180f,0.8494916f, //520
0.9581349f,0.5156323f,0.1127691f,0.008846157f,0.0005584547f, //570
0.00003214816f,0.000001969943f,0.00003153104f,0.003117800f,0.05498056f, //620
0.2950505f,0.9399226f,0.8892048f,0.4050320f,0.06871008f, //670
0.004676404f,0.0002891656f,0.00001721687f }; //700


array<float,32> Spectrum::CIE_Z = {
0.8129501f,0.07824999f,0.003900000f,0.0008000000f, //420
0.00002000000f,0.0f,0.0f,0.0f,0.0f, //470
0.001734286f,0.1832568f,1.7758671f,0.4919673f,0.04489859f, //520
0.002196800f,0.0003834667f,0.000001333333f,0.0f,0.0f, //570
0.0f,0.0f,0.005158320f,0.5196948f,1.7059369f, //620
0.3013375f,0.02378716f,0.001711200f,0.0002119600f,0.0f, //670
0.0f,0.0f,0.0f }; //700


Spectrum::Spectrum()
{
	Clear();
}


Spectrum::Spectrum(string filename)
{

	map<float,float> spd;

	regex comment("^#(.|\\s)*");
	regex entry("\\s*(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	
	ifstream file(filename);
	
	while(!file.eof())
	{
		string line;
		getline(file,line);
		
		match_results<string::const_iterator> results;
		
		
		if(regex_match(line,results,entry))
		{
		
			spd[std::stof(results[1])]=std::stof(results[2]);
					
		}
		
	}
	
	file.close();
	
	
	cout<<"Parsed SPD:"<<endl;
	
	for(pair<float,float> q : spd)
	{
		cout<<q.first<<":"<<q.second<<endl;  
	}
	
	
	
	/*
	 there is room for improvment here
	 as left and right sides not covered by input spd can be 0.0
	 and may be should be interpolated from near points
	*/
	int wl = Spectrum::lambdaStart;
	int N = 0;
	
	map<float,float>::iterator p1;
	map<float,float>::iterator p2;
	
	p1 = spd.begin();
	p2 = spd.begin();
	p2++;
	
	while(p2!=spd.end())
	{
		float wl1=p1->first;
		float wl2=p2->first;
		
		if(wl1< wl && wl2>=wl)
		{
			float dist = wl2 - wl1;
			float center = wl - wl1;
			
			float f = center / dist;
			
			cout<<"["<<wl1<<","<<wl<<","<<wl2<<"]"<<endl;
			
			this->data[N]=p1->second + ((p2->second-p1->second)*f);
			N++;
			wl = wl + Spectrum::lambdaStep;
		}
		else
		{
			p1++;
			p2++;
		}
	}
	
	

}

void Spectrum::Clear()
{
	for(int n=0;n<32;n++)
	{
		data[n]=0.0f;
	}
}

string Spectrum::ToString()
{

	stringstream ss;

	int wl = Spectrum::lambdaStart;
	
	for(int n=0;n<32;n++)
	{
		ss<<"["<<wl<<"]= "<<data[n]<<endl;
		
		wl=wl+Spectrum::lambdaStep;
	}
	
	return ss.str();
}

Spectrum operator+(Spectrum a,Spectrum & b)
{
	Spectrum ret;
	
	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] + b.data[n];
	}
	
	return ret;
}


Spectrum operator-(Spectrum a,Spectrum & b)
{
	Spectrum ret;

	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] - b.data[n];
	}
	
	return ret;
}

Spectrum operator*(Spectrum a,Spectrum & b)
{
	Spectrum ret;

	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] * b.data[n];
	}
	
	return ret;
}

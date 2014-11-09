
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

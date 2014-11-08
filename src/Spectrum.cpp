
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "Spectrum.hpp"

using namespace std;
using namespace com::toxiclabs::iris;


SampledSpectrum::SampledSpectrum()
{
	Clear();
}


SampledSpectrum::SampledSpectrum(string filename)
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
	
	int wl = SampledSpectrum::lambdaStart;
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
			
			this->data[N]=p1->second + (dist*f);
			N++;
			wl = wl + SampledSpectrum::lambdaStep;
		}
		else
		{
			p1++;
			p2++;
		}
	}
	
	
	cout<<"Computed SPD:"<<endl;
	
	wl = SampledSpectrum::lambdaStart;
	for(int n=0;n<32;n++)
	{
		cout<<"["<<wl<<"]="<<data[n]<<endl;
	
		wl = wl + SampledSpectrum::lambdaStep;
	}

}

void SampledSpectrum::Clear()
{
	for(int n=0;n<32;n++)
	{
		data[n]=0.0f;
	}
}

string SampledSpectrum::ToString()
{
	string ret="";

	int wl = SampledSpectrum::lambdaStart;
	
	for(int n=0;n<32;n++)
	{
		
	}
	
	return ret;
}

SampledSpectrum operator+(SampledSpectrum a,SampledSpectrum & b)
{
	SampledSpectrum ret;
	
	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] + b.data[n];
	}
	
	return ret;
}


SampledSpectrum operator-(SampledSpectrum a,SampledSpectrum & b)
{
	SampledSpectrum ret;

	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] - b.data[n];
	}
	
	return ret;
}

SampledSpectrum operator*(SampledSpectrum a,SampledSpectrum & b)
{
	SampledSpectrum ret;

	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] * b.data[n];
	}
	
	return ret;
}

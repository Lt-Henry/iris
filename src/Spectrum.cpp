
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


#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>

#include "Spectrum.hpp"

using namespace std;
using namespace com::toxiclabs::iris;


SampledSpectrum::SampledSpectrum()
{
	Clear();
}


SampledSpectrum::SampledSpectrum(string filename)
{

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

	int wl = SampledSpectrum::lambdaStart;
	
	for(int n=0;n<32;n++)
	{
		
	}
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

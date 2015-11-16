
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>

#include "Spectrum.hpp"
#include "Math.hpp"

using namespace std;
using namespace com::toxiclabs::iris;

float Spectrum::X [32]= {
0.003769647f,0.02214302f,0.08953803f,0.2035729f,0.2918246f,
0.3482554f,0.3224637f,0.2485254f,0.1806905f,0.08182895f,
0.02083981f,0.002461588f,0.01556989f,0.07962917f,0.1818026f,
0.3098117f,0.4494206f,0.6133784f,0.796775f,0.9638388f,
1.109767f,1.151033f,1.083928f,0.9142877f,0.6924717f,
0.4731224f,0.2997374f,0.1707914f,0.09224597f,0.04710606f,
0.02262306f,0.01096778f
}; //700


float Spectrum::Y [32]= {
0.0004146161f,0.002452194f,0.00907986f,0.02027369f,0.03319038f,
0.05033657f,0.06472352f,0.08514816f,0.1298957f,0.1788048f,
0.237916f,0.3483536f,0.5204972f,0.718089f,0.8575799f,
0.9544675f,0.9890228f,0.9967737f,0.9732611f,0.8963613f,
0.8115868f,0.6918553f,0.5583746f,0.4229897f,0.2980865f,
0.1943124f,0.119312f,0.06671045f,0.03559982f,0.01807939f,
0.008661284f,0.004195941f
}; //700


float Spectrum::Z [32]= {
0.0184726f,0.109609f,0.4508369f,1.051821f,1.552826f,
1.917479f,1.848545f,1.522157f,1.25061f,0.7552379f,
0.4099313f,0.2376753f,0.1176796f,0.05650407f,0.02438164f,
0.00984647f,0.003790291f,0.001432128f,0.0005452416f,0.0002117772f,
8.494468e-05f,3.549661e-05f,1.554631e-05f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f
}; //700


float Spectrum::yint=11.2964057f;



Spectrum::Spectrum()
{
	Clear();
}


Spectrum::Spectrum(string filename)
{

	Clear();

	map<float,float> spd;

	regex comment("^#(.|\\s)*");
	regex entry("\\s*(\\d+\\.\\d+|\\d+)\\s+(-?\\d+\\.\\d+)\\s*");
	
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
	
	
	int wl = Spectrum::lambdaStart;
	
	for(int n=0;n<32;n++)
	{
		map<float,float>::iterator left=spd.end();
		map<float,float>::iterator right=spd.end();
	
		map<float,float>::iterator it;
		
		for(it=spd.begin();it!=spd.end();it++)
		{
			if(it->first>=wl)
			{
				if(right!=spd.end())
				{
					if(it->first<right->first)
					{
						right=it;
					}
				}
				else
				{
					right=it;
				}
			}
			
			if(it->first<=wl)
			{
				if(left!=spd.end())
				{
					if(it->first>left->first)
					{
						left=it;
					}
				}
				else
				{
					left=it;
				}
			}
		}
		
		
		
		if(left==spd.end() || right==spd.end())
		{
			data[n]=0.0f;
		}
		else
		{
			float wl1,wl2;
			
			wl1=left->first;
			wl2=right->first;
			
			float dist=wl2 - wl1;
			float center=wl - wl1;
			float f=center/dist;
			float v1=left->second;
			float v2=right->second;
			
			if(AproxToZero(dist))
			{
				data[n]=v1;
			}
			else
			{
				data[n]=v1+((v2-v1)*f);
			}
		}
		
		wl=wl+Spectrum::lambdaStep;
	}	
	
	

}

Spectrum::Spectrum(vector<float> & values)
{

	int n=0;
	
	for(float value : values)
	{
		if(n==32)
			break;
		
		data[n]=value;
		n++;
	}
	
}


Spectrum::Spectrum(int K)
{
	
	/* Plank constant */
	const float h=6.626E-34;
	
	/* Blotzman constant */
	const float k=1.38E-23;
	
	/* Light speed */
	const float c=3E8;
	
	float maxE=0.0f;
	float values[32];
	
	int wl = Spectrum::lambdaStart;
	
	for(int n=0;n<32;n++)
	{
		/*
						 A    C
			E(lambda,T)=--- x---
						 B    D
		*/
		float E;
		float lambda,T;
		float A,B,C,D;
		float ex;
		
		lambda=wl;
		T=K;
		
		
		A=2.0f*h*std::pow(c,2);
		B=std::pow(lambda,5);
		C=1.0f;
		
		ex = (h*c) / (lambda*k*T);
		D=std::exp(ex) -1.0f;
		
		E=(A/B)*(C/D);
		
		cout<<A<<","<<B<<","<<C<<","<<D<<endl;
		cout<<"lambda: "<<lambda<<" E="<<E<<endl;
		
		if(E>maxE)
		{
			maxE=E;
		}
		
		values[n]=E;
		
		wl+=Spectrum::lambdaStep;
	}
	
	/* normalization step */
	for(int n=0;n<32;n++)
	{
		data[n]=values[n]/maxE;
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

ColorXYZ Spectrum::ToXYZ()
{
	
	ColorXYZ color;
	
	color.Black();
		
	for(int n=0;n<32;n++)
	{
		color.x+=Spectrum::X[n]*data[n];
		color.y+=Spectrum::Y[n]*data[n];
		color.z+=Spectrum::Z[n]*data[n];
	}
	
	color.x/=Spectrum::yint;
	color.y/=Spectrum::yint;
	color.z/=Spectrum::yint;

	return color;
}



Spectrum com::toxiclabs::iris::operator+(Spectrum a,Spectrum & b)
{
	Spectrum ret;
	
	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] + b.data[n];
	}
	
	return ret;
}


Spectrum com::toxiclabs::iris::operator-(Spectrum a,Spectrum & b)
{
	Spectrum ret;

	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] - b.data[n];
	}
	
	return ret;
}

Spectrum com::toxiclabs::iris::operator*(Spectrum a,Spectrum & b)
{
	Spectrum ret;

	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n] * b.data[n];
	}
	
	return ret;
}

Spectrum com::toxiclabs::iris::operator*(Spectrum a,float b)
{
	Spectrum ret;
	
	
	for(int n=0;n<32;n++)
	{
		ret.data[n]=a.data[n]*b;
	}
	
	return ret;
}



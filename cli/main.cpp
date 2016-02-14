
#include "Settings.hpp"
#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


int main(int argc,char * argv[])
{

	cout<<"Iris Test"<<endl;
	
	Settings settings;
	Settings custom;
	
	
	settings.Set("camera.x",0.0f);
	settings.Set("camera.y",0.0f);
	settings.Set("camera.z",3.0f);
	
	custom.Set("camera.x",1.0f);
	

	settings.Merge(custom);
	
	vector<string> keys;
	
	keys=settings.GetKeys();
	
	for(string key: keys)
	{
		ValueFloat * v=static_cast<ValueFloat *>(settings.Get(key));
		cout<<"key:"<<v->Get()<<endl;
	}
	
	
	return 0;
}

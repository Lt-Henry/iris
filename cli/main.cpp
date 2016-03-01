
#include "Settings.hpp"
#include "Atmosphere.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


int main(int argc,char * argv[])
{

	cout<<"Iris Test"<<endl;
	
	Settings settings;

	Atmosphere atmosphere(settings);

	return 0;
}

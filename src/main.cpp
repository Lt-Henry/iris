
#include "Core.hpp"

#include <iostream>
#include <string>
#include <stdexcept>




using namespace std;
using namespace com::toxiclabs::iris;


int main(int argc,char * argv[])
{
	cout<<"Iris path-tracer"<<endl;

	if(argc<2)
		throw runtime_error("Missing parameter <dir>");


	Core * core;
	
	core = Core::Init(string(argv[1]));
	
	core->Run();
	
	Core::Quit();

	
	return 0;
}


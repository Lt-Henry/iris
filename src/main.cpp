
#include "Script.hpp"

#include <iostream>
#include <string>
#include <exception>




using namespace std;
using namespace com::toxiclabs::iris;


int main(int argc,char * argv[])
{
	cout<<"Iris path-tracer"<<endl;

	if(argc<2)
		throw runtime_error("Missing parameter <script>");

	Script::Init(string(argv[1]));
	Script::Run();
	Script::Quit();

/*
	Core core(argc,argv);
	
	core.Run();
*/	
	return 0;
}


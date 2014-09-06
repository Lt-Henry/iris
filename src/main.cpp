

#include <iostream>
#include "Core.hpp"


using namespace std;
using namespace com::toxiclabs::iris;


int main(int argc,char * argv[])
{
	cout<<"Iris path-tracer"<<endl;
	
	Core core(argc,argv);
	
	core.Run();
	
	return 0;
}

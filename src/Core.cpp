
#include <iostream>
#include "Core.hpp"
#include "MeshLoader.hpp"

using namespace std;
using namespace com::toxiclabs::iris;


Core::Core(int argc,char * argv[])
{
	if(argc<2)
	{
		cout<<"Not enough parameters"<<endl;
	}
	else
	{
		cout<<"* loading:"<<argv[1]<<endl;
		MeshLoader::Load(argv[1]);
	}
}

Core::~Core()
{
	cout<<"* closing..."<<endl;
}

void Core::Run()
{
	cout<<"* running..."<<endl;
	
}

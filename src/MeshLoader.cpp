
#include "MeshLoader.hpp"

#include <string>
#include <fstream>
#include <regex>
#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;



void MeshLoader::Load(string filename)
{
	ifstream file(filename);
	
	while(!file.eof())
	{
		string line;
		getline(file,line);
		
		if(line[0]=='o')cout<<line<<endl;
	}
	
	file.close();
}

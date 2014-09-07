
#include "MeshLoader.hpp"

#include <string>
#include <fstream>
#include <regex>
#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;



void MeshLoader::Load(string filename)
{

	//regex patterns
	regex comment("^#(.|\\s)*");
	regex usemtl("^usemtl\\s+([a-zA-Z]+)");
	regex mtllib("^mtllib\\s+(.+)");
	regex o("^o\\s+(.+)");
	regex v("^v\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	regex vn("^vn\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	regex vt("^vt\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	regex f("^f\\s+(\\d+)/(\\d+|)/(\\d+|)\\s+(\\d+)/(\\d+|)/(\\d+|)\\s+(\\d+)/(\\d+|)/(\\d+|)");
	
	ifstream file(filename);
	
	while(!file.eof())
	{
		string line;
		getline(file,line);
		
		match_results<string::const_iterator> results;
		
		if(regex_match(line,comment))
			cout<<"comment:"<<line<<endl;		
				
		if(regex_match(line,results,usemtl))
		{
			cout<<"usemtl:"<<results[1]<<endl;
		}
		
		if(regex_match(line,results,mtllib))
		{
			cout<<"mtllib:"<<results[1]<<endl;
		}
		
		
		if(regex_match(line,results,o))
		{
			cout<<"Object:"<<results[1]<<endl;
		}
		
		if(regex_match(line,results,f))
		{
			cout<<"Face"<<endl;
			for(int n=1;n<results.size();n++)
			{
				cout<<results[n]<<endl;
			}
		}

	}
	
	file.close();
}

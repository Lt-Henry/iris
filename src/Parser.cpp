

#include "Parser.hpp"


using namespace com::toxiclabs::iris::parser;
using namespace std;


vector<string> com::toxiclabs::iris::parser::Split(string str,char separator)
{
	vector<string> ret;
	string tmp;
	
	
	for(char c:str)
	{
		if(c!=separator)
		{
			tmp=tmp+c;
		}
		else
		{
			if(tmp!="")
			{
				ret.push_back(tmp);
				tmp="";
			}
		}
	}
	
	if(tmp!="")
	{
		ret.push_back(tmp);
	}
	
	return ret;
}


Rule::Rule(vector<string> & options)
{
	this->options=options;
}

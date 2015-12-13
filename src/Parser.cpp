

#include "Parser.hpp"
#include <iostream>
#include <cctype>

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
			
			ret.push_back(tmp);
			tmp="";
		
		}
	}
	
	if(tmp!="")
	{
		ret.push_back(tmp);
	}
	
	return ret;
}

vector<string> com::toxiclabs::iris::parser::GetTokens(string str)
{
	vector<string> tokens;
	string tmp="";
	bool block=false;
	
	for(char c:str)
	{
		if(c=='#')
		{
			break;
		}
		
		if(block==false)
		{
			if(c!=' ' && c!='\t' && c!='\n')
			{
				tmp=tmp+c;
				if(c=='"')
				{
					block=true;
				}
			}
			else
			{
				if(tmp!="")
				{
					tokens.push_back(tmp);
					tmp="";
				}
			}
		}
		else
		{
			tmp=tmp+c;
			if(c=='"')
			{
				block=false;
				tokens.push_back(tmp);
				tmp="";
			}
		}
	}
	
	if(tmp!="")
	{
		tokens.push_back(tmp);
	}
	
	return tokens;
}



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




void Grammar::AddToken(string name,string token)
{
	tokens[name]=token;
}

void Grammar::AddRule(string name,string rule,function<void(string)> hook)
{
	map<string,Rule>::iterator it;
	
	it=rules.find(name);
	
	if(it==rules.end())
	{
		Rule r;
		r.options[rule]=hook;
		rules[name]=r;
	}
	else
	{
		it->second.options[rule]=hook;
	}
	
}


void Grammar::Push(string text)
{
	vector<string> lex;
	
	lex = parser::Split(text);
	
	
}


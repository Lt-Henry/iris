

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

bool Grammar::IsRule(string name)
{
	bool ret=true;
	
	for(char c: name)
	{
		if(isupper(c))
		{
			ret=false;
			break;
		}
	}
	
	return ret;
}

bool Grammar::IsToken(std::string name)
{
	bool ret=true;
	
	for(char c: name)
	{
		if(islower(c))
		{
			ret=false;
			break;
		}
	}
	
	return ret;
}


void Grammar::FindFirsts(string rule)
{
	vector<string> tmp;
	
	//epsilon
	if(rule=="")
	{
		return;
	}
	
	tmp=Split(rule);
	
	if(IsToken(tmp[0]))
	{
		cout<<"first: "<<tmp[0]<<endl;
	}
	else
	{
		Rule r = rules[tmp[0]];
		
		for(pair<string,function<void(string l)> > alpha : r.options)
		{
			FindFirsts(alpha.first);
		}
	}
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

void Grammar::Build()
{
	for(pair<string,Rule> rule : rules)
	{
		cout<<"rule: "<<rule.first<<endl;
		for(pair<string,function<void(string l)> > alpha : rule.second.options)
		{
			//cout<<"alpha: "<<alpha.first<<endl;
			FindFirsts(alpha.first);
		}
	}
}

void Grammar::Push(string text)
{
	vector<string> lex;
	
	lex = parser::Split(text);
	
	
}


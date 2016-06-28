/*
	Iris raytracer
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Parser.hpp"

#include <iostream>
#include <cctype>


using namespace std;


vector<string> Iris::Parser::Split(string str,char separator)
{
	vector<string> ret;
	string tmp;
	
	
	for (char c:str) {
		if (c!=separator) {
			tmp=tmp+c;
		}
		else {
			
			ret.push_back(tmp);
			tmp="";
		}
	}
	
	if (tmp!="") {
		ret.push_back(tmp);
	}
	
	return ret;
}

vector<string> Iris::Parser::GetTokens(string str)
{
	vector<string> tokens;
	string tmp="";
	bool block=false;
	
	for (char c:str) {
		if (c=='#') {
			break;
		}
		
		if (block==false) {
			if (c!=' ' && c!='\t' && c!='\n') {
				tmp=tmp+c;
				if (c=='"') {
					block=true;
				}
			}
			else {
				if (tmp!="") {
					tokens.push_back(tmp);
					tmp="";
				}
			}
		}
		else {
			tmp=tmp+c;
			if (c=='"') {
				block=false;
				tokens.push_back(tmp);
				tmp="";
			}
		}
	}
	
	if (tmp!="") {
		tokens.push_back(tmp);
	}
	
	return tokens;
}

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

#include "Settings.hpp"


using namespace std;
using namespace Settings;

Value::Value()
{
	this->type=Type::None;
}


Value::Value(Type type)
{
		this->type=type;
}


Value::~Value()
{
}


string Value::signature()
{
	return string("n");
}


ValueInteger::ValueInteger(int data):Value(Type::Integer)
{
	this->data=data;
}


int ValueInteger::get()
{
	return data;
}

string ValueInteger::signature()
{
	return string("i");
}


ValueFloat::ValueFloat(float data):Value(Type::Float)
{
	this->data=data;
}


float ValueFloat::get()
{
	return data;
}


string ValueFloat::signature()
{
	return string("f");
}


ValueString::ValueString(string data):Value(Type::String)
{
	this->data=data;
}


string ValueString::get()
{
	return data;
}


string ValueString::signature()
{
	return string("s");
}


ValueBoolean::ValueBoolean(bool data):Value(Type::Boolean)
{
	this->data=data;
}


bool ValueBoolean::get()
{
	return data;
}


string ValueBoolean::signature()
{
	return string("b");
}


DataSet::DataSet()
{
}


DataSet::~DataSet()
{
	for (pair<string,Value* > p: values)	{
		delete p.second;
	}
}


void DataSet::set(string key,Value * value)
{
	map<string,Value *>::iterator it;
	
	it = values.find(key);
	
	if (it!=values.end()) {
		values.erase(it);
		delete it->second;
	}
	
	values[key]=value;
}


void DataSet::set(string key,int value)
{
	set(key,new ValueInteger(value));
}


void DataSet::set(string key,float value)
{
	set(key,new ValueFloat(value));
}


void DataSet::set(string key,string value)
{
	set(key,new ValueString(value));
}


void DataSet::set(string key,bool value)
{
	Set(key,new ValueBoolean(value));
}


Value * DataSet::get(string key)
{
	map<string,Value*>::iterator it;

	it=values.find(key);

	if (it==values.end()) {
		return nullptr;
	}
	else {
		return it->second;
	}

}


int DataSet::get(std::string key,int value)
{
	Value * v = get(key);

	if (v==nullptr) {
		return value;
	}
	else {
		return static_cast<ValueInteger*>(v)->get();
	}
}


float DataSet::get(std::string key,float value)
{
	Value * v = get(key);

	if (v==nullptr) {
		return value;
	}
	else {
		return static_cast<ValueFloat*>(v)->get();
	}
}


string DataSet::get(string key,string value)
{
	Value * v = get(key);
	
	if (v==nullptr) {
		return value;
	}
	else {
		return static_cast<ValueString*>(v)->get();
	}

}


bool DataSet::get(string key,bool value)
{
	Value * v = get(key);

	if (v==nullptr) {
		return value;
	}
	else {
		return static_cast<ValueBoolean*>(v)->get();
	}
}


vector<string> DataSet::keys()
{
	vector<string> ret;

	for (pair<string,Value*> v:values) {
		ret.push_back(v.first);
	}
	
	return ret;
}

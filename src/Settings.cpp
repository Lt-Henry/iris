
#include "Settings.hpp"


using namespace std;
using namespace com::toxiclabs::iris;

Value::Value()
{
	this->type=ValueType::None;
}


Value::Value(ValueType type)
{
		this->type=type;
}

Value::~Value()
{
}

ValueType Value::GetType()
{
	return type;
}

string Value::GetSignature()
{
	return string("n");
}

ValueInteger::ValueInteger(int data):Value(ValueType::Integer)
{
	this->data=data;
}

int ValueInteger::Get()
{
	return data;
}

string ValueInteger::GetSignature()
{
	return string("i");
}

ValueFloat::ValueFloat(float data):Value(ValueType::Float)
{
	this->data=data;
}

float ValueFloat::Get()
{
	return data;
}

string ValueFloat::GetSignature()
{
	return string("f");
}


ValueString::ValueString(string data):Value(ValueType::String)
{
	this->data=data;
}

string ValueString::Get()
{
	return data;
}

string ValueString::GetSignature()
{
	return string("s");
}

ValueBoolean::ValueBoolean(bool data):Value(ValueType::Boolean)
{
	this->data=data;
}

bool ValueBoolean::Get()
{
	return data;
}

string ValueBoolean::GetSignature()
{
	return string("b");
}

Settings::Settings()
{
}

Settings::~Settings()
{
	for(pair<string,Value* > p: values)
	{
		//delete p.second;
		cout<<"deleting: "<<p.first<<endl;
	}
}

void Settings::Set(string key,Value * value)
{
	map<string,Value *>::iterator it;
	
	it = values.find(key);
	
	if(it!=values.end())
	{
		values.erase(it);
		delete it->second;
	}
	
	values[key]=value;
}

void Settings::Set(string key,int value)
{
	Set(key,new ValueInteger(value));
}

void Settings::Set(string key,float value)
{
	Set(key,new ValueFloat(value));
}

void Settings::Set(string key,string value)
{
	Set(key,new ValueString(value));
}

void Settings::Set(string key,bool value)
{
	Set(key,new ValueBoolean(value));
}

Value * Settings::Get(string key)
{
	map<string,Value*>::iterator it;

	it=values.find(key);

	if(it==values.end())
	{
		return nullptr;
	}
	else
	{
		return *it;
	}

}

int Settings::Get(std::string key,int value)
{
	Value * v = Get(key);

	if(v==nullptr)
	{
		return value;
	}
	else
	{
		return static_cast<ValueInteger*>(v)->Get();
	}
}

float Settings::Get(std::string key,float value)
{
	Value * v = Get(key);

	if(v==nullptr)
	{
		return value;
	}
	else
	{
		return static_cast<ValueFloat*>(v)->Get();
	}
}

string Settings::Get(string key,string value)
{
	Value * v = Get(key);
	
	if(v==nullptr)
	{
		return value;
	}
	else
	{
		return static_cast<ValueString*>(v)->Get();
	}

}

bool Settings::Get(string key,bool value)
{
	Value * v = Get(key);

	if(v==nullptr)
	{
		return value;
	}
	else
	{
		return static_cast<ValueBoolean*>(v)->Get();
	}
}

vector<string> Settings::GetKeys()
{
	vector<string> keys;

	for(pair<string,Value*> v:values)
	{
		keys.push_back(v.first);
	}
	
	return keys;
}

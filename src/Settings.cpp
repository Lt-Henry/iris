
#include "Settings.hpp"


using namespace std;
using namespace com::toxiclabs::iris;


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


void Settings::Set(string key,int value)
{
	values[key]=ValueInteger(value);
}

void Settings::Set(string key,float value)
{
	values[key]=ValueFloat(value);
}

void Settings::Set(string key,string value)
{
	values[key]=ValueString(value);
}

void Settings::Set(string key,bool value)
{
	values[key]=ValueBoolean(value);
}

Value Settings::Get(string key)
{
	return values[key];
}

void Settings::Merge(Settings & settings)
{
	
}

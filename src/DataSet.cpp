

#include "DataSet.hpp"


using namespace std;
using namespace com::toxiclabs::iris;


DataSet::DataSet()
{
	type=DataType::None;
}

DataSet::~DataSet()
{
}

DataSet::DataSet(int data)
{
	Set(data);
}

DataSet::DataSet(float data)
{
	Set(data);
}

DataSet::DataSet(string data)
{
	Set(data);
}

void DataSet::Set(int data)
{
	type=DataType::Integer;
	data_i=data;
}

void DataSet::Set(float data)
{
	type=DataType::Float;
	data_f=data;
}

void DataSet::Set(string data)
{
	type=DataType::String;
	data_s=data;
}

int DataSet::GetInt()
{
	return data_i;
}

float DataSet::GetFloat()
{
	return data_f;
}

string DataSet::GetString()
{
	return data_s;
}

void DataSet::operator=(const int & data)
{
	Set(data);
}

void DataSet::operator=(const float & data)
{
	Set(data);
}

void DataSet::operator=(const string & data)
{
	Set(data);
}




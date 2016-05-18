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

#ifndef _IRIS_SETTINGS_
#define _IRIS_SETTINGS_

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <iostream>


namespace Settings
{
	enum class Type {
		None,
		Integer,
		Float,
		String,
		Boolean
	};
	
	
	class Value {
	public:
		Type type;
		
		Value();
		Value(Type type);
		virtual ~Value();
		
		
		virtual std::string signature();
	};
	
	
	class ValueInteger: public Value {
	protected:
		
		int data;
		
	public:
		
		ValueInteger(int data);
		
		int get();
		
		std::string signature();
		
	};
	
	
	class ValueFloat: public Value {
	protected:
				
		float data;
		
	public:
		
		ValueFloat(float data);
		

		float get();
		
		std::string signature();
	};
	
	
	class ValueString: public Value {
	protected:
				
		std::string data;
				
	public:
				
		ValueString(std::string data);
		
		std::string get();
		
		std::string signature();
	};
	
	
	class ValueBoolean: public Value {
	protected:
				
		bool data;
		
	public:
		
		ValueBoolean(bool data);
		
		bool get();
		
		std::string signature();
	};
	
	
	class DataSet {
	protected:
			
		std::map<std::string,Value *> values;
		
	public:
		
		DataSet();
		~DataSet();
		
		void set(std::string key,Value * value);
		void set(std::string key,int value);
		void set(std::string key,float value);
		void set(std::string key,std::string value);
		void set(std::string key,bool value);
		
		Value * get(std::string key);
		
		int get(std::string key,int value);
		float get(std::string key,float value);
		std::string get(std::string key,std::string value);
		bool get(std::string key,bool value);

		std::vector<std::string> keys();
	};
			
}



#endif

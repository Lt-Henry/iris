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

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			enum class ValueType
			{
				None,
				Integer,
				Float,
				String,
				Boolean
			};
			
			class Value
			{
				protected:
				
				ValueType type;
				
				public:
				
				Value();
				Value(ValueType type);
				virtual ~Value();
				
				ValueType GetType();
				
				virtual std::string GetSignature();
			};
			
			class ValueInteger: public Value
			{
				protected:
				
				int data;
				
				public:
				
				ValueInteger(int data);
				
				int Get();
				
				std::string GetSignature();
				
			};
			
			class ValueFloat: public Value
			{
				protected:
				
				float data;
				
				public:
				
				ValueFloat(float data);
				~ValueFloat()
				{
					std::cout<<"float destructor"<<std::endl;
				};


				float Get();
				
				std::string GetSignature();
			};
			
			class ValueString: public Value
			{
				protected:
				
				std::string data;
				
				public:
				
				ValueString(std::string data);
				
				std::string Get();
				
				std::string GetSignature();
			};
			
			class ValueBoolean: public Value
			{
				protected:
				
				bool data;
				
				public:
				
				ValueBoolean(bool data);
				
				bool Get();
				
				std::string GetSignature();
			};
			
			class Settings
			{
				protected:
				
				std::map<std::string,Value *> values;
				
				public:
				
				Settings();
				~Settings();
				
				void Set(std::string key,Value * value);
				void Set(std::string key,int value);
				void Set(std::string key,float value);
				void Set(std::string key,std::string value);
				void Set(std::string key,bool value);
				
				Value * Get(std::string key);
				
				int Get(std::string key,int value);
				float Get(std::string key,float value);
				std::string Get(std::string key,std::string value);
				bool Get(std::string key,bool value);

				std::vector<std::string> GetKeys();
			};
		
		}
	}
}

#endif

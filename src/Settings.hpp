#ifndef _IRIS_SETTINGS_
#define _IRIS_SETTINGS_

#include <cstdint>
#include <string>

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
				
				std::map<std::string,Value> values;
				
				public:
				
				Settings();
				
				void Set(std::string key,int value);
				void Set(std::string key,float value);
				void Set(std::string key,std::string value);
				void Set(std::string key,bool value);
				
				Value Get(std::string key);
				
				void Merge(Settings & settings);
			};
		
		}
	}
}

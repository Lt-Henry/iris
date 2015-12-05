#ifndef _IRIS_PARSER_
#define _IRIS_PARSER_

#include <vector>
#include <map>
#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			namespace parser
			{
				std::vector<std::string> Split(std::string str,char separator=' ');
				
				class Rule
				{
					public:
					
					std::vector<std::string> options;
					
					Rule(std::vector<std::string> & options);
				};
				
				
				class Grammar
				{
					private:
					
					std::map<std::string,std::string> tokens;
					std::map<std::string,Rule> rules;
					
					public:
					
					
					void AddToken(std::string name,std::string token);
					void AddRule(std::string name,Rule rule);
					
				};
			}
		}
	}
}


#endif

#ifndef _IRIS_PARSER_
#define _IRIS_PARSER_

#include <vector>
#include <map>
#include <functional>
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
					
					std::map<std::string,std::function<void(std::string l)> > options;
					
					
				};
				
				
				class Grammar
				{
					private:
					
					std::map<std::string,std::string> tokens;
					std::map<std::string,Rule> rules;
					
					
					bool IsRule(std::string name);
					bool IsToken(std::string name);
					
					void FindFirsts(std::string rule);
					
					public:
					
					
					void AddToken(std::string name,std::string token);
					void AddRule(std::string name,std::string rule,std::function<void(std::string l)> hook);
					void Build();
					void Push(std::string line);
				};
			}
		}
	}
}


#endif

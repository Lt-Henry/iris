#ifndef _IRIS_PARSER_
#define _IRIS_PARSER_

#include <vector>
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
				
				std::vector<std::string> GetTokens(std::string str);
				
				
			}
		}
	}
}


#endif


#ifndef _IRIS_SCRIPT_
#define _IRIS_SCRIPT_

#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Script
			{
			
				private:
				
				static Script * instance;
				
				Script(std::string filename);
				~Script();
				
				public:
				
				
				static Script * Get();
				static void Init(std::string filename);
				static void Quit();
			};
		}
	}
}

#endif

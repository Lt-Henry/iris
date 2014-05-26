

#ifndef _IRIS_CORE_
#define _IRIS_CORE_

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Core
			{
				public:
				
				/**
				 * constructor
				 */ 
				Core(int argc,char * argv[]);
				
				/**
				 * destructor
				 */ 
				~Core();
				
				/**
				 * Starts the system
				 */ 
				void Run();
			};
		}
	}
}


#endif

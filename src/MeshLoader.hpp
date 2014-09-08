

#ifndef _IRIS_MESHLOADER_
#define _IRIS_MESHLOADER_

#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class MeshLoader
			{
				public:
				
				static void Load(std::string filename);
				
				private:
				
				static void LoadMaterialLib(std::string filename);
			};
		}
	}
}


#endif

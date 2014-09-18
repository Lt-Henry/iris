

#ifndef _IRIS_MESHLOADER_
#define _IRIS_MESHLOADER_

#include "Scene.hpp"

#include <string>
#include <vector>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class MeshLoader
			{
				public:
				
				static void Load(std::string filename,std::vector<Triangle*> & triangles, std::vector<Material *> & materials);
				
				private:
				
				static void LoadMaterialLib(std::string filename);
			};
		}
	}
}


#endif



#ifndef _IRIS_MESHLOADER_
#define _IRIS_MESHLOADER_

#include "Mesh.hpp"
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
				
				static void LoadOBJ(std::string filename);
				static void LoadSTL(std::string filename);
				
				static void Load(std::string filename,std::vector<Geometry*> & geometries, std::vector<Material *> & materials);
				
				private:
				
				static void LoadMaterialLib(std::string filename);
			};
		}
	}
}


#endif

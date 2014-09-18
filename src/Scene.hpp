
#ifndef _IRIS_SCENE_
#define _IRIS_SCENE_

#include "Material.hpp"
#include "Mesh.hpp"

#include <vector>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Scene
			{
				public:
				
				std::vector<Triangle *> triangles;
				std::vector<Material *> materials;
						
				
			};
		}
	}
}
#endif

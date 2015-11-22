#ifndef _IRIS_SCENE_GRAPH_
#define _IRIS_SCENE_GRAPH_

#include "Mesh.hpp"
#include <set>
#include <vector>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class SceneGraph
			{
				public:
				
				virtual ~SceneGraph() { };
				
				virtual void Traverse(Vector & origin, Vector & direction,std::vector<Geometry *> & geometries)=0;
			};
		}
	}
}


#endif

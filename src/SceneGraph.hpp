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
				
				virtual ~SceneGraph()=0;
				
				virtual std::set<Geometry *> Traverse(Vector & origin, Vector & direction)=0;
			};
		}
	}
}


#endif

#ifndef _IRIS_SCENE_GRAPH_
#define _IRIS_SCENE_GRAPH_

#include "Mesh.hpp"
#include <vector>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class SceneNode
			{
				public:
				
				std::vector<Geometry *> geometries;
			};
			
			class SceneGraph
			{
				public:
				
				
				virtual ~SceneGraph() { };
				
				virtual void Traverse(Vector & origin, Vector & direction,std::vector<SceneNode *> & nodes)=0;
			};
		}
	}
}


#endif


#ifndef _IRIS_KDTREE_
#define _IRIS_KDTREE_

#include "Mesh.hpp"

#include <vector>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
		
			enum class KdNodeType
			{
				SplitX=0,
				SplitY=1,
				SplitZ=2,
				Child=3	
			};
			
			class KdNode
			{
				public:
				
				KdNodeType type;
				
				float partition;
				
				std::vector<Triangle *> triangles;
				
				KdNode * left;
				KdNode * right;
			};
			
			class KdTree
			{
				public:
				
				KdNode * root;
				int small;
								
				
				KdTree(std::vector<Triangle *> & triangles);
				~KdTree();
				
				void Build(KdNode * node,std::vector<Triangle *> & triangles);
				
				void Free();
				void Free(KdNode * node);
				
				void Traverse(Vector & origin,Vector & destination,KdNode * node);
			};
		}
	}
}

#endif

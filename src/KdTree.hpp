
#ifndef _IRIS_KDTREE_
#define _IRIS_KDTREE_

#include "SceneGraph.hpp"
#include "Mesh.hpp"

#include <vector>
#include <list>

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
			
			class KdNode : public SceneNode
			{
				public:
				
				KdNodeType type;
				
				float partition;
				
				
				KdNode * left;
				KdNode * right;
				
				BoundBox aabb;
				
				bool RayCollision(Vector & origin,Vector & direction);
			};
			
			
			class KdTree : public SceneGraph
			{
			
				private:
				
				KdNode * root;
				KdNode * special_node;
				
				int small;
				
				void AddRange(std::list<std::pair<float,float>> & ranges,std::pair<float,float> range);
				void Build(KdNode * node,std::vector<Geometry *> & geometries);
				
				void Free();
				void Free(KdNode * node);
				
				void Traverse(Vector & origin,Vector & direction,
					KdNode * node,std::vector<SceneNode *> & nodes);
				
				public:
				
				KdTree(std::vector<Geometry *> & geometries);
				~KdTree();
				
				/*! overloaded method */
				void Traverse(Vector & origin,Vector & direction,std::vector<SceneNode *> & nodes);
				
				
			};
		}
	}
}

#endif

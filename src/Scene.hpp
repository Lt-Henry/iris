
#ifndef _IRIS_SCENE_
#define _IRIS_SCENE_

#include "Material.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "DataSet.hpp"

#include <vector>
#include <map>
#include <string>

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
				std::map<std::string,DataSet> params;
				
				Camera * camera;
				
				Scene();
				
				void SetCamera(Camera * camera);
				void ApplyCamera();
				
			};
		}
	}
}
#endif

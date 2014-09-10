

#ifndef _IRIS_MESH_
#define _IRIS_MESH_

#include <vector>
#include <array>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Triangle
			{
				public:
				
				std::array<int,3> vertices;
				std::array<int,3> normals;
				
				int material;
			};
			
			class Mesh
			{
				public:
				
				std::vector<float> vertices;
				std::vector<float> normals;
				
				std::vector<Triangle> triangles;
			};
		}
	}
}


#endif

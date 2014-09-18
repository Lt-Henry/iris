

#ifndef _IRIS_MESH_
#define _IRIS_MESH_

#include "Math.hpp"

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
				
				std::array<Vector,3> vertices;
				std::array<Vector,3> normals;
				
				int material;
			};
		}
	}
}


#endif

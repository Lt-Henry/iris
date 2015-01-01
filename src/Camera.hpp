
#ifndef _IRIS_CAMERA_
#define _IRIS_CAMERA_

#include "Math.hpp"

#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Camera
			{
				public:
				
				std::string name;
				Vector position;
				Vector target;
				float angle;
				float near;
				float far;
				
				Matrix matrix;
				
				Camera();
				Camera
				(std::string  name,Vector & position,
				Vector & target,float angle=0.0f,float near=5.0f,float far=100.0f);
				
				
				~Camera();
				
				void CreateMatrix();
			};
		}
	}
}


#endif

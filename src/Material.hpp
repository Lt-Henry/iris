
#ifndef _IRIS_MATERIAL_
#define _IRIS_MATERIAL_

#include "Spectrum.hpp"

#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Material
			{
				public:
				
				std::string name;
				Spectrum Kd;	
				
			};
		}
	}
}


#endif

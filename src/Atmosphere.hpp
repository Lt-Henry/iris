
#ifndef _IRIS_ATMOSPHERE_
#define _IRIS_ATMOSPHERE_

#include "Settings.hpp"
#include "Spectrum.hpp"
#include "Math.hpp"

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Atmosphere
			{
				public:
				
				Vector sun_position;
				Spectrum sunlight;
				Spectrum sky;
				
				Atmosphere(Settings & settings);
				
				Spectrum Energy(Vector & direction);
				
				void Mult(Matrix & m);
			};
		}
	}
}

#endif

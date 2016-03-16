#ifndef _IRIS_SENSOR_
#define _IRIS_SENSOR_

#include "Settings.hpp"
#include "Color.hpp"
#include "Spectrum.hpp"

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{

			/*!
				CCD Sensor with Bayer filter
				RG
				GB
			*/



			class Sensor
			{
				private:
				
				//number of pixels
				int width;
				int height;

				//physical sensor size, in mm
				float sensor_width;
				float sensor_height;

				float * buffer;

				public:
				
				Sensor(Settings & settings);

				~Sensor();

				int GetWidth();
				int GetHeight();

				void SetPixel(int x,int y,Spectrum & spr);
			};
		}
	}
}

#endif

#ifndef _IRIS_SENSOR_
#define _IRIS_SENSOR_

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
				
				int width;
				int height;

				float sensor_width;
				float sensor_height;

				float * buffer;

				public:
				
				Sensor(int width,int height,float sensor_width,float sensor_height);

				~Sensor();

				int GetWidth();
				int GetHeight();

				void SetPixel(int x,int y,Spectrum & spr);
			};
		}
	}
}

#endif

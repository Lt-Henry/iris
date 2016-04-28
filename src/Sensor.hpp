#ifndef _IRIS_SENSOR_
#define _IRIS_SENSOR_

#include "Settings.hpp"
#include "Color.hpp"
#include "Spectrum.hpp"
#include "BitMap.hpp"

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
				int cols;
				int rows;

				//physical sensor size, in meters
				float width;
				float height;

				//cell buffer
				float * buffer;
				
				BitMap * bitmap;

				public:
				
				Sensor(Settings & settings);

				~Sensor();

				float GetWidth();
				float GetHeight();
				
				int GetColumns();
				int GetRows();

				void SetCell(int x,int y,Spectrum & spr);
				BitMap * Process();
				
			};
		}
	}
}

#endif

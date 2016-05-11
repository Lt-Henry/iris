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
				
				//shutter exposure time in seconds 
				float exposure_time;
				
				//sensor sinsivity in mV/lux-sec
				float sensivity;
				
				//dark current in mV/sec
				float dark_current;
				
				//dynamic range in Db
				float dynamic_range;

				//cell buffer
				float * buffer;
				
				BitMap * bitmap;
				
				//bayer filter
				Spectrum red;
				Spectrum green;
				Spectrum blue;

				public:
				
				Sensor(Settings & settings);

				~Sensor();


				float GetWidth();
				float GetHeight();
				
				int GetColumns();
				int GetRows();

				void SetCell(int x,int y,Spectrum & spr);
				
				/*! clear sensor */
				void Clear();
				
				/*! simulate a CCD to bitmap process */
				BitMap * Process();
				
			};
		}
	}
}

#endif

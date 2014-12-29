
#ifndef _IRIS_SPECTRUM_
#define _IRIS_SPECTRUM_

#include "Color.hpp"

#include <string>
#include <array>


namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Spectrum
			{
				public:
				
				/* CIE XYZ precomputed tables */
				
				static std::array<float,32> X;
				static std::array<float,32> Y;
				static std::array<float,32> Z;
				static float yint;
				
				/* sampled range [390 - 710[  */
				static const int lambdaStart = 390; 
				static const int lambdaEnd = 710; 
				static const int lambdaStep = 10;
				
				/* using 32 samples, in order to take profit from sse operations*/
				std::array<float,32> data;
				
				Spectrum();
				Spectrum(std::string filename);

				void Clear();
				std::string ToString();
				
				void ToXYZ(Color * color);
				
			};
			
			Spectrum operator+(Spectrum a,Spectrum & b);
			Spectrum operator-(Spectrum a,Spectrum & b);
			Spectrum operator*(Spectrum a,Spectrum & b);
			
		}
	}
}


#endif

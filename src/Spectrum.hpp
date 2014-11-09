
#ifndef _IRIS_SPECTRUM_
#define _IRIS_SPECTRUM_

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
				
				/* sampled range */
				static const int lambdaStart = 380;
				static const int lambdaEnd = 700;
				static const int lambdaStep = 10;
				
				/* using 32 samples, in order to take profit from sse operations*/
				std::array<float,32> data;
				
				Spectrum();
				Spectrum(std::string filename);

				void Clear();
				std::string ToString();
			};
			
			Spectrum operator+(Spectrum a,Spectrum & b);
			Spectrum operator-(Spectrum a,Spectrum & b);
			Spectrum operator*(Spectrum a,Spectrum & b);
			
		}
	}
}


#endif


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
			class SampledSpectrum
			{
				public:
				
				/* sampled range */
				static const int lambdaStart = 380;
				static const int lambdaEnd = 700;
				static const int lambdaStep = 10;
				
				/* using 32 samples, in order to take profit from sse operations*/
				std::array<float,32> data;
				
				SampledSpectrum();
				SampledSpectrum(std::string filename);

				void Clear();
				std::string ToString();
			};
			
			SampledSpectrum operator+(SampledSpectrum a,SampledSpectrum & b);
			SampledSpectrum operator-(SampledSpectrum a,SampledSpectrum & b);
			SampledSpectrum operator*(SampledSpectrum a,SampledSpectrum & b);
			
		}
	}
}


#endif

#include <vector>

#ifndef _IRIS_SPECTRUM_
#define _IRIS_SPECTRUM_

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
				
				/* using 32 samples, in order to profit from sse operations*/
				std::array<float,32> data;
				
				SampledSpectrum();

				void Clear();
			};
			
			SampledSpectrum operator+(SampledSpectrum a,SampledSpectrum & b);
			SampledSpectrum operator-(SampledSpectrum a,SampledSpectrum & b);
			SampledSpectrum operator*(SampledSpectrum a,SampledSpectrum & b);
			
		}
	}
}


#endif

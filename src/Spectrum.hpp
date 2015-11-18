
#ifndef _IRIS_SPECTRUM_
#define _IRIS_SPECTRUM_

#include "Color.hpp"

#include <string>
#include <vector>


namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Spectrum
			{
				public:
				
				/*!
				 *  CIE XYZ precomputed tables 
				 **/
				
				static float X[32];
				static float Y[32];
				static float Z[32];
				static float yint;
				
				/*!
				 *  sampled range [390 - 710[  
				 **/
				static const int lambdaStart = 390; 
				static const int lambdaEnd = 710; 
				static const int lambdaStep = 10;
				
				/*!
				 *  using 32 samples, in order to take profit from sse operations (some day)
				 * */
				float data[32];
				
				/*!
				 * Simple constructor
				 */ 
				Spectrum();
				
				/*!
				 * Creates spectrum from a spd file
				 */ 
				Spectrum(std::string filename);
				
				/*!
				 * Creates a spectrum from a vector of 32 floats
				 */ 
				Spectrum(std::vector<float> & values);
				
				/*!
					Create a spectrum given a temperature K
					using Planck Radiation law
					
					url:http://csep10.phys.utk.edu/astr162/lect/light/radiation.html
				*/
				Spectrum(int K);
				
				/*!
				 * Set all values to zero
				 */ 
				void Clear();
				
				/*!
				 * Converts to string
				 */ 
				std::string ToString();
				
				
				/*!
				 * Converts spectrum to a XYZ color representation
				 */ 
				ColorXYZ ToXYZ();
				
				/*!
				 * Normalize spectrum
				 */ 
				 void Normalize();
				
			};
			
			Spectrum operator+(Spectrum a,Spectrum & b);
			Spectrum operator-(Spectrum a,Spectrum & b);
			Spectrum operator*(Spectrum a,Spectrum & b);
			Spectrum operator*(Spectrum a,float b);
			
		}
	}
}


#endif

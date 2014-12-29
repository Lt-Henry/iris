
#ifndef _IRIS_COLOR_
#define _IRIS_COLOR_

#include <cstdint>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Color
			{
				public:
				
				union
				{
					struct
					{
						float r;
						float g;
						float b;
						float a;
					};
					
					struct
					{
						float x;
						float y;
						float z;
						float w;
					};
					
					float data[4];	
				};
				
				void Clear();
				void Clamp();
				uint32_t ToPixelRGBA();
				
				Color XYZtoRGB();			
			};
		}
	}
}

#endif

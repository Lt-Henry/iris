
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
				
				
				virtual void Black()=0;
				virtual void White()=0;
				
				virtual void Clamp()=0;
				
			};
			
			
			class ColorRGB : public Color
			{
				public:
				
				void Black();
				void White();
				void Clamp();
				
				uint32_t ToPixel();
				
			};

			class ColorXYZ : public Color
			{
				public:
				
				void Black();
				void White();
				void Clamp();
				
				ColorRGB ToRGB();
			};
			
			
		}
	}
}

#endif

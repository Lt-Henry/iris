
#ifndef _IRIS_COLOR_
#define _IRIS_COLOR_

#include <cstdint>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
		
			enum class ColorType
			{
				None,
				RGB,
				XYZ,
				HSV
			};
			
			class Color
			{
				public:
				
								
				ColorType type;
				
				virtual void Black()=0;
				virtual void White()=0;
				
				virtual void Clamp()=0;
				
				virtual ~Color(){};
				
				ColorType GetType();
			};
			
			
			class ColorRGB : public Color
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
					
					float data[4];
				};

				ColorRGB();
				ColorRGB(float r,float g,float b,float a=1.0f);
				
				void Black();
				void White();
				void Clamp();
				void Clamp(float min,float max);
				
				uint32_t ToPixel();
				
			};
			
			ColorRGB operator * (ColorRGB c,float f);
			ColorRGB operator / (ColorRGB c,float f);

			class ColorXYZ : public Color
			{
				public:
				
				union
				{
					struct
					{
						float x;
						float y;
						float z;
					};
					
					float data[4];
				};
				
				ColorXYZ();
				ColorXYZ(float x,float y,float z);
				
				void Black();
				void White();
				void Clamp();
				
				ColorRGB ToRGB();
			};
			
			class ColorHSV : public Color
			{
				public:
				
				union
				{
					struct
					{
						float h;
						float s;
						float v;
					};
					
					float data[4];
				};

				ColorHSV();
				ColorHSV(float h,float s,float v);

				void Black();
				void White();
				void Clamp();

				ColorRGB ToRGB();

			};
		}
	}
}

#endif

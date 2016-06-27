/*
	Iris raytracer
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _IRIS_COLOR_
#define _IRIS_COLOR_

#include <cstdint>

namespace Iris
{
	namespace Color
	{
		enum class Type {
			None,
			RGB,
			XYZ,
			HSV
		};
	
		/*
			Virtual color class
		*/
		class Base {
		public:
	
		Type type;
	
		/*
			Sets to black
		*/
		virtual void Black()=0;
	
		/*
			Sets to white
		*/
		virtual void White()=0;

		/*
			clamp to the valid ranges
		*/
		virtual void Clamp()=0;


		virtual ~Base(){};
	
		};
	
	
		/*
			Tristimulus color space
		*/
		class XYZ : public Base {
		public:
				
			union {
				struct {
					float x;
					float y;
					float z;
					float pad;
				};
		
				float data[4];
			};
	
			XYZ();
			XYZ(float x,float y,float z);
	
			void Black();
			void White();
			void Clamp();
	
		};
	
	
		/*
			Hue Saturation Value color space
		*/
		class HSV : public Base {
		public:
		
			union {
				struct {
					float h;
					float s;
					float v;
					float pad;
				};
			
				float data[4];
			};

			HSV();
			HSV(float h,float s,float v);

			void Black();
			void White();
			void Clamp();

		};
	
	
		/*
			Red Green Blue representation
		*/
		class RGB : public Base {
		public:
	
			union {
				struct {
					float r;
					float g;
					float b;
					float a;
				};
		
				float data[4];
			};

			/*
				Simple constructor
			*/
			RGB();
	
			/*
				Creates a rgba color, input ranges expected in [0,1]
			*/
			RGB(float r,float g,float b,float a=1.0f);
		

			/*
				Creates a rgba color from a xyz
			*/
			RGB(XYZ & xyz);
			
			/*
				Creates a rgba color from a hsv
			*/	
			RGB(HSV & hsv);
		

	
			void Black();
			void White();
			void Clamp();
	
			/*
				clamp in the min,max range
			*/
			void Clamp(float min,float max);
	
			/*
				returns a rgba 32 bit encoded pixel
			*/
			uint32_t Pixel();
	
		};
	
		/*
			RGB product
		*/
		RGB operator * (RGB c,float f);
	
		/*
			RGB division
		*/
		RGB operator / (RGB c,float f);
	}
}
#endif

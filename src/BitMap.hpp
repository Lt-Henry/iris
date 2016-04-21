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

#ifndef _IRIS_BITMAP_
#define _IRIS_BITMAP_

#include "Color.hpp"

#include <cstdint>
#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class BitMap
			{
				private:
				
				/* RGBA buffer */
				uint8_t * buffer;
				
				int width;
				int height;
			
				public:
				
				BitMap(int width,int height);
				~BitMap();
				
				void PutPixel(int x,int y,ColorRGB & color);
				
				void Save(std::string & filename);
				
			};
		}
	}
}


#endif

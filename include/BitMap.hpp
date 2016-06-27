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

namespace Iris
{
	/*
		RGBA 32bit pixel bit map
	*/
	class BitMap {
	private:
	
		/* RGBA buffer */
		uint8_t* buffer;
	
		int w;
		int h;

	public:
	
		/*
			Creates a bitmap with given dimensions
		*/
		BitMap(int width,int height);
	
		/*
			Destroys the bitmap (and allocated structures)
		*/
		~BitMap();
	
		/*
			gets width
		*/
		int Width();
	
		/*
			gets height
		*/
		int Height();
	
		/*
			Puts a rgb pixel
		*/
		void PutPixel(int x,int y,Color::RGB & color);
	
		/*
			Reads a RGB pixel
		*/
		Color::RGB GetPixel(int x,int y);
	
		/*
			Save as a png image
		*/
		void Save(std::string filename);
	
	};
}
#endif

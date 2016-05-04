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


#include "BitMap.hpp"

#include <png.h>

#include <cstdio>
#include <stdexcept>
#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;




BitMap::BitMap(int width,int height)
{
	this->width=width;
	this->height=height;
	
	buffer=new uint8_t[width*height*4];
}


BitMap::~BitMap()
{
	delete buffer;
}

int BitMap::GetWidth()
{
	return width;
}

int BitMap::GetHeight()
{
	return height;
}

void BitMap::PutPixel(int x,int y,ColorRGB & color)
{
	int index;
	
	index = (x*4)+(y*width*4);
	
	color.Clamp();
	
	buffer[index+0]=255.0f*color.r;
	buffer[index+1]=255.0f*color.g;
	buffer[index+2]=255.0f*color.b;
	buffer[index+3]=255.0f*color.a;
}


ColorRGB BitMap::GetPixel(int x,int y)
{
	ColorRGB color;
	
	int index;
	
	index = (x*4)+(y*width*4);
	
	color.r=buffer[index+0]/255.0f;
	color.g=buffer[index+1]/255.0f;
	color.b=buffer[index+2]/255.0f;
	color.a=buffer[index+3]/255.0f;
	
	return color;
}

void BitMap::Save(string filename)
{
	FILE * fp;
	
	fp=fopen(filename.c_str(),"wb");
	
	if(!fp)
	{
		throw runtime_error("Failed to open file");
	}
	
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png)
	{
		throw runtime_error("Failed to create PNG struct");
	}

	png_infop info = png_create_info_struct(png);
	
	if (!info)
	{
		throw runtime_error("Failed to create PNG info struct");
	}
	
	// I hate this shit 
	if (setjmp(png_jmpbuf(png)))
	{
		throw runtime_error("Failed to write PNG");
	}
	
	png_init_io(png, fp);
	
	png_set_IHDR(
		png,
		info,
		width, height,
		8,
		PNG_COLOR_TYPE_RGB_ALPHA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	
	png_write_info(png, info);

	png_bytepp row_pointers;
	
	//create a list of row pointers
	row_pointers=new png_bytep[height];

	for(int n=0;n<height;n++) 
	{
		row_pointers[n]=buffer+(width*4*n);
	}

	
	png_write_image(png, row_pointers);
	png_write_end(png, nullptr);

	fclose(fp);

	delete row_pointers;
}

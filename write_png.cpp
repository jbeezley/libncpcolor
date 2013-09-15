/*
Copyright 2013 Jonathan Beezley <jon.beezley@gmail.com>

This file is part of libncpcolor.

libncpcolor is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libncpcolor is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser Public License for more details.

You should have received a copy of the GNU Lesser Public License
along with libncpcolor.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "write_png.h"

#include <png.h>
#include <cstddef>
#include <cstdlib>

#include<iostream>

bool write_png(const string& filename, const size_t width, const size_t height, const uint8_t *raster) {
    
    FILE *fp = fopen(filename.c_str(), "wb");
    if(!fp) return false;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        fclose(fp);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        fclose(fp);
        png_destroy_write_struct(&png_ptr,NULL);
        return false;
    }

    if(setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr,&info_ptr);
        fclose(fp);
        return false;
    }
    png_init_io(png_ptr, fp);
    
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
   
    png_write_info(png_ptr, info_ptr);
    
    png_bytep row_pointer[height];
    
    for(size_t i=0; i<height; i++) {
        row_pointer[i] = (png_bytep) &(raster[i*width*4]);
        //png_write_row(png_ptr, row_pointer);
    }
    png_set_rows(png_ptr, info_ptr, &row_pointer[0]);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
    return true;
}


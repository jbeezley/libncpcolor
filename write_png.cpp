
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
    cout << "png_init_io" << endl;
    png_init_io(png_ptr, fp);
    cout << "png_set_IHDR" << endl;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
   
    cout << "png_write_png" << endl;
    
    png_write_info(png_ptr, info_ptr);
    //png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    png_bytep row_pointer;
    
    for(size_t i=0; i<height; i++) {
    cout << "png_write_row " << i << endl;
        row_pointer = (png_bytep) &(raster[i*width*4]);
        png_write_row(png_ptr, row_pointer);
    }

    cout << "png_write_end" << endl;
    png_write_end(png_ptr, info_ptr);
    cout << "png_destroy_write" << endl;
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
    return true;
}


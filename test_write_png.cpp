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


#include <string>
#include <iostream>

#include "write_png.h"

uint8_t pixel_foo(const int i) {
    int j = (i % 512) - 256;
    if(j<0) j = -j;
    return ((size_t) j) % 256;
}

bool write_image(const string& filename, const size_t width, const size_t height, const uint8_t *pixel) {
    const size_t size = width*height;
    int ix,iy,j; 
    uint8_t *raster = new uint8_t [size*4];

    for(size_t i=0; i<size; i++) {
        if(pixel) {
            raster[4*i + 1] = pixel[0];
            raster[4*i + 2] = pixel[1];
            raster[4*i + 3] = pixel[2];
        }
        else {
            ix = i % 256;
            iy = i - ix;
            j = ix + iy;
            raster[4*i + 1] = pixel_foo(j);
            raster[4*i + 2] = pixel_foo(3*j);
            raster[4*i + 3] = pixel_foo(5*j);
        }
        raster[4*i] = 255;
    }
    if(write_png(filename, width, height, raster)) {
        cout << "Successfully wrote " + filename << endl;
    }
    else {
        cout << "Failed writing " + filename << endl;
        exit(1);
    }
    delete [] raster;
    return true;
}

int main() {
    size_t width, height; 
    const
    uint8_t red[]   = {255, 0, 0, 255},
            green[] = {0, 255, 0, 255},
            blue[]  = {0, 0, 255, 255},
            grey[]  = {127, 127, 127, 255};
    string filename;

    const size_t sizes[][2] = { { 256, 256 },
                              { 512, 512 },
                              { 1024, 512 },
                              { 2048, 1024 },
                              { 511, 247 } };
    const char *snames[] = { "256_256", 
                            "512_512",
                            "1024_1024",
                            "2048_1024",
                            "511_247" };


    for(int is = 0; is < 5; is++) {
        width = sizes[is][0];
        height = sizes[is][1];

        filename = "red";
        filename += snames[is];
        filename += ".png";
        write_image(filename, width, height, red);

        filename = "green";
        filename += snames[is];
        filename += ".png";
        write_image(filename, width, height, green);
        
        filename = "blue";
        filename += snames[is];
        filename += ".png";
        write_image(filename, width, height, blue);
        
        filename = "grey";
        filename += snames[is];
        filename += ".png";
        write_image(filename, width, height, grey);
        
        filename = "gradient";
        filename += snames[is];
        filename += ".png";
        write_image(filename, width, height, NULL);
    }


    exit(0);
}

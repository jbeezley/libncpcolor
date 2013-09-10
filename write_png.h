
#ifndef WRITE_PNG_H
#define WRITE_PNG_H

#include <string>

using namespace std;

bool write_png(const string& filename, const size_t width, const size_t height, const uint8_t *raster);

#endif

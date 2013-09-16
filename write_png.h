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



#ifndef WRITE_PNG_H
#define WRITE_PNG_H

#include <string>

/*! \file
 *  \brief A simple wrapper around libpng to write a png image.
 */

/*!
 * Write a pseudo color image like that created by LookupTable::makePColor into a png file.
 * \param[in] filename The file name to write the image to
 * \param[in] width The width of the image in pixels
 * \param[in] height The height of the image in pixels
 * \param[in] raster A pointer to the data
 * \retval true if write successful
 * \retval false if write failed
 * \sa LookupTable::makePColor
 */
bool write_png(const std::string& filename, const size_t width, const size_t height, const uint8_t *raster);

#endif

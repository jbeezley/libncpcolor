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

#ifndef _PCOLOR_H
#define _PCOLOR_H

#include<cstddef>
#include<stdint.h>
#include<string>
#include<map>

/*!
 * \file 
 *
 * \brief Defines a color lookup table for \p uint8_t values.
 *
 * This header defines the class LookupTable which serves to 
 * map \p uint8_t values into RGBA color pixels stored as
 * <tt>uint8_t [4]</tt>.
 * The primary method for interacting with this class is to 
 * retrieve predefined lookup tables via the static methods
 * LookupTable::getLUT.  The lookup table can then operate
 * on a \p uint8_t array to produce a raster with the method
 * LookupTable::makePColor.
 */

/*!
 * \class LookupTable
 * \brief Defines a mapping from [0,255] to RGBA colors.
 *
 * This class provides support for converting an array of values into
 * a pseudocolor image.  The table itself is a mapping from \p uint8_t -> <tt>uint8_t [4]</tt>,
 * where the output format is determined by the static const values
 * LookupTable::LUT_RED, LookupTable::LUT_GREEN, LookupTable::LUT_BLUE, and LookupTable::LUT_ALPHA.  These
 * values have been set to be compatible with the Qt::QImage::fromData
 * method.  By design, an arbitrary typed array can be normalized using a
 * (sub)class element of type LinearNorm.  The normalized array is then
 * passed to LookupTable::makePColor to generate a pseudocolor raster image.
 *
 * The intended method for interacting with this class is to load predefined lookup
 * tables using the static LookupTable::getLUT methods; however, it is possible to load additional
 * lookup tables from files or data using LookupTable::setData and LookupTable::readData.
 */
class LookupTable {
public:
    static const unsigned int LUTROWS = 256, /*!< Number of rows in a lookup table */
                              LUTCOLORS = 3, /*!< Number of colors in a lookup table */
                              LUTSIZE = LUTROWS * LUTCOLORS, /*!< Total size of a lookup table */
                              PIXELELEMENTS = 4; /*!< Number of elements in an output pixel */

    static const unsigned int LUT_RED = 2, /*!< Position of red in a pixel */
                              LUT_GREEN = 1, /*!< Position of green in a pixel */
                              LUT_BLUE = 0, /*!< Position of blue in a pixel */
                              LUT_ALPHA = 3; /*!< Position of alpha value in a pixel */

    static const unsigned int READ_LUT_RED = 0, /*!< Position of red in a lookup table */
                              READ_LUT_GREEN = 1, /*!< Position of green in a lookup table */
                              READ_LUT_BLUE = 2; /*!< Position of blue in a lookup table */

private:
    //! To contain the main lookup table
    uint8_t lut[LUTSIZE];

    //! Pointers to individual components
    uint8_t *lut_r, *lut_g, *lut_b;

    //! Flag to reverse the lookup table
    bool _reverse;

    //! Flag if data is set yet.
    bool initialized;

    // static elements for loading predefined lookup tables
    static bool lutmap_init; /*!< To keep track of if the predefined lookup tables have been loaded */
    static std::map<std::string, const LookupTable*> lutmap; /*!< Contains all predefined lookup tables */
    static std::map<std::string, const LookupTable*> lutmap_r; /*!< Contains reversed predefined lookup tables */
    static void loadAll(); /*!< Static function to load all lookup tables from lutData.h */


public:

    /*! Default constructor */
    LookupTable();

    /*! Copy constructor */
    LookupTable(const LookupTable& other);
    
    /*! Get the number of \p uint8_t elements per pixel. */
    static size_t pixelSize() { return PIXELELEMENTS; }

    /*! Get the total size of an image with N pixels.  For allocation of the
     * pseudo-color array in makePColor()
     *  \param[in] N Total number of pixels of the image
     *  \return Total size of the image needed for makePColor()
     */
    static size_t imageSize(const size_t N) { return pixelSize() * N; }
    
    /*! Set the lookup table data.
     *  \param[in] lutData \p uint8_t array of size LookupTable::LUTSIZE with
     *  LookupTable::LUTROWS rows and LookupTable::LUTCOLORS columns.  The 
     *  order of the colors in the table are given by LookupTable::READ_LUT_RED, 
     *  LookupTable::READ_LUT_GREEN, and LookupTable::READ_LUT_BLUE.  This array
     *  is copied into the private member variable \p lut.
     *  \sa readData()
     */
    void setData(const uint8_t lutData[]);

    /*! Set the lookup table data from a file.
     *  \param[in] fileName The name of a file containing binary data in the format
     *  described in setData().
     *  \retval true if the file was read successfully
     *  \retval false if an error occured*/
    bool readData(const std::string& fileName);

    /*! Return a pointer to the lookup table data. */
    const uint8_t* getData() { return lut; }
    
    /*! Convert a normalized array into color pixels.
     * \param[in] N Size of the input array
     * \param[in] TArray Input array
     * \param[out] PArray Output array of size <tt>LookupTable::imageSize(N)</tt>*/
    void makePColor(const size_t N, const uint8_t TArray[], uint8_t PArray[]) const;
    
    /*! Load a predefined lookup table of name \p tableName
     * \retval true on success
     * \retval false if the table was not found*/
    bool loadTable(const std::string& tableName);

    /*! Set the reverse flag.  The lookup table will be inverted: i -> 255 - i*/
    void setReverse(bool reverse) { _reverse = reverse; }
    
    /*! Get a statically allocated reference to a predefined lookup table.
     * \param tableName The name of the table
     * \param reversed When true, return the inverted version of tableName 
     * \sa getLUT(const int, bool) */
    static const LookupTable& getLUT(const std::string& tableName, bool reversed = false);

    /*! Get a statically allocated reference to a predefined lookup table.
     * \param iLUT The number of the lookup table to fetch (0 <= iLUT <= getNTables())
     * \param reversed When true, return the inverted version of tableName 
     * \sa getLUT(const std::string&, bool) */
    static const LookupTable& getLUT(const int iLUT, bool reversed = false);

    /*! Return the total number of predefined tables. */
    static int getNTables();

    /*! Return the name of the ith lookup table. */
    static const char* getTableName(const int i);

};

#endif

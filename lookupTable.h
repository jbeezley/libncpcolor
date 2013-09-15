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

using namespace std;

class LookupTable {
public:
    static const unsigned int LUTROWS = 256,
                              LUTCOLORS = 3,
                              LUTSIZE = LUTROWS * LUTCOLORS,
                              PIXELELEMENTS = 4;

    static const unsigned int LUT_RED = 2, 
                              LUT_GREEN = 1,
                              LUT_BLUE = 0,
                              LUT_ALPHA = 3;

    static const unsigned int READ_LUT_RED = 0, 
                              READ_LUT_GREEN = 1,
                              READ_LUT_BLUE = 2;

private:
    // To contain the main lookup table
    uint8_t lut[LUTSIZE];

    // Pointers to individual components
    uint8_t *lut_r, *lut_g, *lut_b;
    bool _reverse;

    // Flag if data is set yet.
    bool initialized;

    // static elements for loading predefined lookup tables
    static bool lutmap_init;
    static map<string, const LookupTable*> lutmap;
    static map<string, const LookupTable*> lutmap_r;
    static void loadAll();


public:
    LookupTable();
    LookupTable(const LookupTable& other);
    
    static size_t pixelSize() { return PIXELELEMENTS; }
    static size_t imageSize(const size_t N) { return pixelSize() * N; }

    void setData(const uint8_t lutData[]);
    bool readData(const string& fileName);
    const uint8_t* getData() { return lut; }
     
    void makePColor(const size_t N, const uint8_t TArray[], uint8_t PArray[]) const;
    bool loadTable(const string& tableName);
    void setReverse(bool reverse) { _reverse = reverse; }

    static const LookupTable& getLUT(const string& tableName, bool reversed = false);
    static const LookupTable& getLUT(const int iLUT, bool reversed = false);
    static int getNTables();
    static const char* getTableName(const int i);

};

#endif

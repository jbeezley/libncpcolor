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


#include "lookupTable.h"
#include <fstream>

#include "lutData.h"
#include <cassert>

using namespace std;

bool LookupTable::lutmap_init = false;
map<string, const LookupTable*> LookupTable::lutmap;
map<string, const LookupTable*> LookupTable::lutmap_r;

LookupTable::LookupTable() {
    lut_r = &(lut[LUTROWS * READ_LUT_RED]);
    lut_g = &(lut[LUTROWS * READ_LUT_GREEN]);
    lut_b = &(lut[LUTROWS * READ_LUT_BLUE]);
    initialized = 0;
    _reverse = false;
}

LookupTable::LookupTable(const LookupTable &other) {
    for(size_t i = 0; i<LUTSIZE; i++) lut[i] = other.lut[i];
    lut_r = &(lut[LUTROWS * READ_LUT_RED]);
    lut_g = &(lut[LUTROWS * READ_LUT_GREEN]);
    lut_b = &(lut[LUTROWS * READ_LUT_BLUE]);
    initialized = other.initialized;
    _reverse = other._reverse;
}

void LookupTable::setData(const uint8_t lutData[]) {
    uint8_t *ir = lut_r, *ig = lut_g, *ib = lut_b;

    for(unsigned int i = 0; i < LUTROWS; i++) {
        *(ir++) = lutData[i * LUTCOLORS + READ_LUT_RED];
        *(ig++) = lutData[i * LUTCOLORS + READ_LUT_GREEN];
        *(ib++) = lutData[i * LUTCOLORS + READ_LUT_BLUE];
    }
    initialized = 1;
}

bool LookupTable::readData(const string& fileName) {
    ifstream file(fileName.c_str(), ios::in | ios::binary | ios::ate);
    if( !file.is_open() ) return false;
    unsigned int size = (unsigned int) file.tellg();
    file.seekg(0, ios::beg);
    if( size != LUTSIZE * sizeof(uint8_t) ) return false;
    file.read((char*)lut, size);
    file.close();
    initialized = true;
    return true;
}

void LookupTable::makePColor(const size_t N, const uint8_t TArray[], uint8_t PArray[]) const {
    assert(initialized);
    if(!_reverse) {
        for(size_t i=0; i<N; i++) {
            PArray[i*pixelSize() + LUT_ALPHA] = 255;
            PArray[i*pixelSize() + LUT_RED]   = lut_r[TArray[i]]; 
            PArray[i*pixelSize() + LUT_GREEN] = lut_g[TArray[i]];
            PArray[i*pixelSize() + LUT_BLUE]  = lut_b[TArray[i]];
        }
    }
    else {
        for(size_t i=0; i<N; i++) {
            PArray[i*pixelSize() + LUT_ALPHA] = 255;
            PArray[i*pixelSize() + LUT_RED]   = lut_r[255-TArray[i]]; 
            PArray[i*pixelSize() + LUT_GREEN] = lut_g[255-TArray[i]];
            PArray[i*pixelSize() + LUT_BLUE]  = lut_b[255-TArray[i]];
        }
    }
}

bool LookupTable::loadTable(const string& tableName) {
    for(int i=0; i<lut::NTables; i++) {
        if((string)lut::lookupTables[i].name == tableName) {
            setData((uint8_t*)lut::lookupTables[i].data);
            return true;
        }
    }
    return false;
}

const LookupTable& LookupTable::getLUT(const string &tableName, bool reversed) {
    if(!lutmap_init) loadAll();
    if(!reversed)
        return *lutmap[tableName];
    else
        return *lutmap_r[tableName];
}

const LookupTable& LookupTable::getLUT(const int iLUT, bool reversed) {
    assert(iLUT >=0 && iLUT < getNTables());
    return getLUT((string) getTableName(iLUT), reversed);
}

void LookupTable::loadAll() {
    LookupTable *lut_p, *lut_r;
    std::string lutName;
    for(int i=0; i<lut::NTables; i++) {
        lut_p = new LookupTable();
        lut_r = new LookupTable();
        lutName = lut::lookupTables[i].name;
        lut_p->loadTable(lutName);
        lut_r->loadTable(lutName);
        lut_r->setReverse(true);
        lutmap[string(lut::lookupTables[i].name)] = lut_p;
        lutmap_r[string(lut::lookupTables[i].name)] = lut_r;
    }
    lutmap_init = true;
}

int LookupTable::getNTables() {
    return lut::NTables;
}

const char* LookupTable::getTableName(const int i) {
    return lut::lookupTables[i].name;
}

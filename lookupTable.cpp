#include "lookupTable.h"
#include <fstream>

#include "lutData.h"
#include <cassert>

LookupTable::LookupTable() {
    lut_r = &(lut[LUTROWS * READ_LUT_RED]);
    lut_g = &(lut[LUTROWS * READ_LUT_GREEN]);
    lut_b = &(lut[LUTROWS * READ_LUT_BLUE]);
    initialized = 0;
    _reverse = false;
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
    return true;
}

void LookupTable::makePColor(const size_t N, const uint8_t TArray[], uint8_t PArray[]) const {
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

#include "lookupTable.h"
#include <fstream>

LookupTable::LookupTable() {
	lut_r = &(lut[LUTROWS * LUT_RED]);
	lut_g = &(lut[LUTROWS * LUT_GREEN]);
	lut_b = &(lut[LUTROWS * LUT_BLUE]);
	initialized = 0;
}

void LookupTable::setData(const uint8_t lutData[]) {
	uint8_t *ir = lut_r, *ig = lut_g, *ib = lut_b;

	for(unsigned int i = 0; i < LUTROWS; i++) {
		*(ir++) = lutData[i + LUTSIZE * LUT_RED];
		*(ig++) = lutData[i + LUTSIZE * LUT_GREEN];
		*(ib++) = lutData[i + LUTSIZE * LUT_BLUE];
	}
	initialized = 1;
}

bool LookupTable::readData(const string& fileName) {
	ifstream file(fileName.c_str(), ios::in | ios::binary | ios::ate);
	if( !file.is_open() ) return false;
	unsigned int size = (unsigned int) file.tellg();
	file.seekg(0, ios::beg);
	if( size != LUTROWS * LUTCOLORS * sizeof(uint8_t) ) return false;
	file.read((char*)lut, size);
	file.close();
	return true;
}

template <typename T>
bool LookupTable::makePColor(const size_t n, const T TArray[], uint8_t PArray[]) const {
	return true;
}

template bool LookupTable::makePColor(const size_t, const float [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const double [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const short [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const int [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const long [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const unsigned short [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const unsigned int [], uint8_t []) const;
template bool LookupTable::makePColor(const size_t, const unsigned long [], uint8_t []) const;

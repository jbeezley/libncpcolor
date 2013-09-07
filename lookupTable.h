
#ifndef _PCOLOR_H
#define _PCOLOR_H

#include<cstddef>
#include<stdint.h>
#include<string>

using namespace std;

class LookupTable {

public:
	LookupTable();
	
	void setData(const uint8_t lutData[]);
	bool readData(const string& fileName);
		
	template <typename T>
	bool makePColor(const size_t n, const T TArray[], uint8_t PArray[]) const;
private:
	static const unsigned int LUTROWS = 256,
				              LUTCOLORS = 3,
							  LUTSIZE = LUTROWS * LUTCOLORS;
	static const unsigned int LUT_RED = 0, 
				              LUT_GREEN = 1,
							  LUT_BLUE = 2;
	
	// To contain the main lookup table
	uint8_t lut[LUTSIZE];

	// Pointers to individual components
	uint8_t *lut_r, *lut_g, *lut_b;

	// Flag if data is set yet.
	bool initialized;
};

#endif

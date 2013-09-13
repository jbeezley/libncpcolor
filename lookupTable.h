
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

    static const unsigned int LUT_RED = 0, 
                              LUT_GREEN = 1,
                              LUT_BLUE = 2,
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
    static map<string, LookupTable> lutmap;
    static map<string, LookupTable> lutmap_r;
    static void loadAll();


public:
    LookupTable();
    
    static size_t pixelSize() { return PIXELELEMENTS; }
    static size_t imageSize(const size_t N) { return pixelSize() * N; }

    void setData(const uint8_t lutData[]);
    bool readData(const string& fileName);
     
    void makePColor(const size_t N, const uint8_t TArray[], uint8_t PArray[]) const;
    bool loadTable(const string& tableName);
    void setReverse(bool reverse) { _reverse = reverse; }

    static const LookupTable& getLUT(const string& tableName, bool reversed = false);
    static const LookupTable& getLUT(const int iLUT, bool reversed = false);
    static int getNTables();
    static const char* getTableName(const int i);

};

#endif

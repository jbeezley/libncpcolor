
#include <iostream>
#include <string>

#include "ncFileReader.h"
#include "norm.h"
#include "lookupTable.h"
#include "write_png.h"

using namespace std;

int main() {

    // The following are set according the example input file.
    // The structure of this file is as follows:
    /*
netcdf example {
dimensions:
    Time = 2 ;
    Z = 40 ;
    Y = 100 ;
    X = 120 ;
variables:
    float A(Time, Z, Y, X) ;
}
    */
    const string& fileName = "example.nc"; // file name to open
    const string& pngName = "example.png"; // file to write png output
    const string& varName = "A";           // variable in the file
    const string& lutName = "hot";         // lookup table to use
    const int xDim = 2;                    // dimension to show horizontally
    const int yDim = 3;                    // dimension to show vertically
    
    // open the file
    const NcSliceFile file(fileName);
    if(!file.isOpen()) return 1;           // exit if the file was not opened
    
    // get the variable
    const BaseVariable *var = file.getVariable(varName);
    if(!var) return 2;                     // exit if the variable does not exist

    // get the default slice and modify
    BaseVariable::sliceType slice = var->defaultSlice();
    slice.setXDim(xDim);                   // set horizontal dimension
    slice.setYDim(yDim);                   // set vertical dimension
    slice[0] = 1;                          // slice at "Time" = 1
    slice[2] = 10;                         // slice at "X" = 10
    if (!slice.isValid()) return 3;        // exit if the slice is invalid
    
    // allocate memory and read the data  
    const int N = var->sliceSize(slice);
    double *varData = new double[N];
    if(!var->readSlice(slice, varData)) return 4; // exit if read failed
    
    // create LinearNorm instance
    LinearNorm<double> norm;
    norm.setMinMaxValFromArray(N, varData);
    
    // normalize the variable data
    uint8_t *normData = new uint8_t [N];
    norm.normalize(N, varData, normData);
    
    // get the requested predefined lookup table
    const LookupTable &lut = LookupTable::getLUT(lutName);

    // allocate memory and perform the conversion
    uint8_t *imageData = new uint8_t [lut.imageSize(N)];
    lut.makePColor(N, normData, imageData);
    
    // write the data to a png file
    if(!write_png(pngName, var->shape()[xDim], var->shape()[yDim], imageData)) return 5;

    // free all allocated memory
    delete [] imageData;
    delete [] normData; 
    delete [] varData;

    cout << "Success!" << endl;
    return 0;
}

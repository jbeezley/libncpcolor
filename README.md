libncpcolor 
===========

A library for creating pseudo-color images from netCDF files.  See the [documentation](http://jbeezley.github.io/libncpcolor/index.html)

Classes
-------
The user of this library interacts with the following classes:

* NcSliceFile
* BaseVariable
* SliceType
* LinearNorm
* LookupTable

The standard usage involves the following steps:

1.  opening a netCDF file with NcSliceFile
2.  retrieving a pointer to a ::Variable object with NcSliceFile::getVariable
3.  instantiating a SliceType object
4.  reading data from the variable with Variable::readSlice
5.  normalizing the variable's data to a \p uint8_t array using a LinearNorm object
6.  loading a predefined LookupTable with LookupTable::getLUT()
7.  generating a raster image by calling LookupTable::makePColor()
8.  calling write_png to write the image to a png file

See [examples/basic.cpp](http://jbeezley.github.io/libncpcolor/basic_8cpp-example.html) for an example of this procedure.

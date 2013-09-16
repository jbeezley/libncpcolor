libncpcolor 
===========

A library for creating pseudo-color images from netCDF files.  See the [documentation](http://jbeezley.github.io/libncpcolor/index.html).

Classes
-------
The user of this library interacts with the following classes:

* [NcSliceFile](http://jbeezley.github.io/libncpcolor/class_nc_slice_file.html)
* [Variable](http://jbeezley.github.io/libncpcolor/class_nc_slice_file.html)
* [SliceType](http://jbeezley.github.io/libncpcolor/class_slice_type.html)
* [LinearNorm](http://jbeezley.github.io/libncpcolor/class_linear_norm.html)
* [LookupTable](http://jbeezley.github.io/libncpcolor/class_lookup_table.html)

The standard usage involves the following steps:

1.  opening a netCDF file with [NcSliceFile](http://jbeezley.github.io/libncpcolor/class_nc_slice_file.html)
2.  retrieving a pointer to a [Variable](http://jbeezley.github.io/libncpcolor/class_nc_slice_file.html) object with [NcSliceFile::getVariable](http://jbeezley.github.io/libncpcolor/class_nc_slice_file.html#a03ea5c704eff65d78a55f393ddf9b14d)
3.  instantiating a [SliceType](http://jbeezley.github.io/libncpcolor/class_slice_type.html)object
4.  reading data from the variable with [Variable::readSlice](http://jbeezley.github.io/libncpcolor/class_variable.html#a7847212fbaec839b439d6b1757e55b45)
5.  normalizing the variable's data to a <tt>uint8_t</tt> array using a [LinearNorm](http://jbeezley.github.io/libncpcolor/class_linear_norm.html) object
6.  loading a predefined [LookupTable](http://jbeezley.github.io/libncpcolor/class_lookup_table.html) with [LookupTable::getLUT](http://jbeezley.github.io/libncpcolor/class_lookup_table.html#adfce54bdcdcecf5ae2b0e091d3a8de32)
7.  generating a raster image by calling [LookupTable::makePColor](http://jbeezley.github.io/libncpcolor/class_lookup_table.html#a69fb8cbc7c61fe840f6f0e4031e4cf40)
8.  calling [write_png](http://jbeezley.github.io/libncpcolor/write__png_8h.html#a022b4f58ce16a98710dc042ccb1cc927) to write the image to a png file

See [examples/basic.cpp](http://jbeezley.github.io/libncpcolor/basic_8cpp-example.html) for an example of this procedure.

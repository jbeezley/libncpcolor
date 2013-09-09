ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CPPFLAGS = ${NETCDF_CFLAGS}

#bin_PROGRAMS = testLookupTable.exe testNorm.exe testSlice.exe testNcFileReader.exe

lib_LTLIBRARIES = libncpcolor-@LIBNCPCOLOR_API_VERSION@.la
libncpcolor_@LIBNCPCOLOR_API_VERSION@_la_SOURCES = lookupTable.cpp lookupTable.h norm.h \
							 ncFileReader.cpp ncFileReader.h      \
							 netcdf.cpp netcdfCompat.h            \
							 ncvalues.cpp ncvaluesCompat.h
libncpcolor_@LIBNCPCOLOR_API_VERSION@_la_LDFLAGS = -version-info $(LIBNCPCOLOR_SO_VERSION)
libncpcolor_includedir = $(includedir)
nobase_libncpcolor_include_HEADERS = ncFileReader.h lookupTable.h norm.h
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libncpcolor-$(LIBNCPCOLOR_API_VERSION).pc

#testLookupTable_exe_SOURCES = lookupTable.cpp lookupTable.h testLookupTable.cpp
#testNorm_exe_SOURCES = norm.h testNorm.cpp
#testSlice_exe_SOURCES = ncFileReader.cpp ncFileReader.h testSlice.cpp netcdf.cpp ncvalues.cpp
#testSlice_exe_LDADD = $(NETCDF_LIBS)
#testNcFileReader_exe_SOURCES = ncFileReader.cpp ncFileReader.h testNcFileReader.cpp netcdf.cpp ncvalues.cpp
#testNcFileReader_exe_LDADD = $(NETCDF_LIBS)
dist_noinst_SCRIPTS = autogen.sh

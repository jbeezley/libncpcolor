ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CPPFLAGS = ${NETCDF_CFLAGS}

check_PROGRAMS = testLookupTable testNorm testSlice testNcFileReader
bin_PROGRAMS = nc2pcolor

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

TESTS = testLookupTable testNorm testSlice run_testNcFileReader.sh

LDADD = libncpcolor-$(LIBNCPCOLOR_API_VERSION).la $(NETCDF_LIBS)

nc2pcolor_SOURCES = nc2pcolor.cpp
testLookupTable_SOURCES = testLookupTable.cpp
testNorm_SOURCES = testNorm.cpp
testSlice_SOURCES = testSlice.cpp
testNcFileReader_SOURCES = testNcFileReader.cpp
dist_noinst_SCRIPTS = autogen.sh
CLEANFILES = test.nc

lutData.h: luts/* buildluts.py bin.py
	rm -f luts/*.h
	python buildluts.py

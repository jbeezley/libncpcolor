ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CPPFLAGS = ${NETCDF_CFLAGS}

check_PROGRAMS = testLookupTable testNorm testSlice testNcFileReader test_write_png
bin_PROGRAMS = nc2pcolor

lib_LTLIBRARIES = libncpcolor-@LIBNCPCOLOR_API_VERSION@.la
libncpcolor_@LIBNCPCOLOR_API_VERSION@_la_SOURCES = lookupTable.cpp \
							 ncFileReader.cpp ncFileReader.h      \
							 netcdf.cpp netcdfCompat.h            \
							 ncvalues.cpp ncvaluesCompat.h       \
							 lutData.h write_png.h write_png.cpp
libncpcolor_@LIBNCPCOLOR_API_VERSION@_la_LDFLAGS = -version-info $(LIBNCPCOLOR_SO_VERSION)
libncpcolor_includedir = $(includedir)
nobase_libncpcolor_include_HEADERS = ncFileReader.h lookupTable.h norm.h lutData.h luts/*.h
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libncpcolor.pc

TESTS = testLookupTable testNorm testSlice run_testNcFileReader.sh test_write_png

LDADD = libncpcolor-$(LIBNCPCOLOR_API_VERSION).la $(NETCDF_LIBS) $(LIBPNG_LIBS)

nc2pcolor_SOURCES = nc2pcolor.cpp
nc2pcolor_CPPFLAGS = ${LIBPNG_CFLAGS}
testLookupTable_SOURCES = testLookupTable.cpp
testNorm_SOURCES = testNorm.cpp
testSlice_SOURCES = testSlice.cpp
testNcFileReader_SOURCES = testNcFileReader.cpp
test_write_png_SOURCES = test_write_png.cpp
dist_noinst_SCRIPTS = autogen.sh
CLEANFILES = test.nc red*.png green*.png blue*.png grey*.png gradient*.png

lutData.h: luts/* buildluts.py bin.py
	rm -f luts/*.h
	python buildluts.py

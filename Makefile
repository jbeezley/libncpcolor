DEBUG=-O2 #-fbounds-check -g
%.o: %.cpp
	g++ -c $(DEBUG) $<

%.exe: %.o
	g++ $(DEBUG) -o $@ $^ -lnetcdf

libncpcolor.a: lookupTable.o ncFileReader.o netcdf.o ncvalues.o
	ar rcs $@ $^
	ranlib $@

test: testLookupTable testNorm testSlice testNcFileReader

testLookupTable: testLookupTable.exe
	./testLookupTable.exe

testNorm: testNorm.exe
	./testNorm.exe

testSlice: testSlice.exe
	./testSlice.exe

testNcFileReader: testNcFileReader.exe
	./testNcFileReader.exe

lookupTable.o: lookupTable.cpp lookupTable.h
ncFileReader.o: ncFileReader.cpp ncFileReader.h

testLookupTable.o: testLookupTable.cpp lookupTable.h
testLookupTable.exe: testLookupTable.o lookupTable.o
testNorm.o: testNorm.cpp norm.h
testNorm.exe: testNorm.o
testSlice.o: testSlice.cpp ncFileReader.h
testSlice.exe: testSlice.o ncFileReader.o netcdf.o ncvalues.o
ncFileReader.h: netcdfCompat.h ncvaluesCompat.h
testNcFileReader.o: testNcFileReader.cpp ncFileReader.h
testNcFileReader.exe: testNcFileReader.o ncFileReader.o netcdf.o ncvalues.o
netcdf.o: netcdf.cpp netcdfCompat.h
ncvalues.o: ncvalues.cpp ncvaluesCompat.h

clean:
	rm -f *.o *.exe *.a

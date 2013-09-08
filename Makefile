
%.o: %.cpp
	g++ -c $<

%.exe: %.o
	g++ -o $@ $^ -lnetcdf_c++ -lnetcdf


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
testSlice.exe: testSlice.o ncFileReader.o
testNcFileReader.o: testNcFileReader.cpp ncFileReader.h
testNcFileReader.exe: testNcFileReader.o ncFileReader.o

clean:
	rm -f *.o *.exe

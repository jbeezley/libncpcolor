
%.o: %.cpp
	g++ -c $<

%.exe: %.o
	g++ -o $@ $^


test: testLookupTable testNorm

testLookupTable: testLookupTable.exe
	./testLookupTable.exe

testNorm: testNorm.exe
	./testNorm.exe

testSlice: testSlice.exe
	./testSlice.exe

lookupTable.o: lookupTable.cpp lookupTable.h
ncFileReader.o: ncFileReader.cpp ncFileReader.h

testLookupTable.o: testLookupTable.cpp lookupTable.h
testLookupTable.exe: testLookupTable.o lookupTable.o
testNorm.o: testNorm.cpp norm.h
testNorm.exe: testNorm.o
testSlice.o: testSlice.cpp ncFileReader.h
testSlice.exe: testSlice.o ncFileReader.o

clean:
	rm -f *.o *.exe

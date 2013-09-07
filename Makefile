
%.o: %.cpp
	g++ -c $<

%.exe: %.o
	g++ -o $@ $^


test: testLookupTable testNorm

testLookupTable: testLookupTable.exe
	./testLookupTable.exe

testNorm: testNorm.exe
	./testNorm.exe

lookupTable.o: lookupTable.cpp lookupTable.h

testLookupTable.o: testLookupTable.cpp lookupTable.h
testLookupTable.exe: testLookupTable.o lookupTable.o
testNorm.o: testNorm.cpp norm.h
testNorm.exe: testNorm.o

clean:
	rm -f *.o *.exe

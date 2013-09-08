
#include "ncFileReader.h"

#include<iostream>
#include<string>

using namespace std;

int main() {
	string fileName("test.nc");
	NcSliceFile file(fileName);
	cout << file;
}

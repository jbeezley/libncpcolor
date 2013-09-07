#include "lookupTable.h"

#include<iostream>
#include<dirent.h>
#include<string>

using namespace std;

int main() {
	LookupTable lutTable;
	struct dirent *entry;
	DIR *dp;
	string dirname("luts");
	int hasError = 0;

	float A[100];
	uint8_t B[100];

	dp = opendir(dirname.c_str());
	if (dp == NULL) {
		perror("Cannot open luts directory");
		return -1;
	}
	while( (entry = readdir(dp)) ) {
		string dname = entry->d_name;
		if(dname != "." && dname != "..") {
    		if( lutTable.readData( dirname + "/" + dname ) ) {
    			cout << "Successfully read " << dname << endl;
				lutTable.makePColor(100, A, B);
    		}
    		else {
    			cout << "Error reading " << dname << endl;
				hasError++;
    		}
		}
	}
	return hasError;
}

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
   
    const size_t N = 1000;
    uint8_t A[N];
    uint8_t B[LookupTable::imageSize(N)];
    
    for(size_t i=0; i<N; i++) A[i] = i % 256;
    dp = opendir(dirname.c_str());
    if (dp == NULL) {
        perror("Cannot open luts directory");
        return -1;
    }
    while( (entry = readdir(dp)) ) {
        string dname = entry->d_name;
        if(dname != "." && dname != ".." && dname.substr(dname.size()-2) != string(".h") ) {
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

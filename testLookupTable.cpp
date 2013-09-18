/*
Copyright 2013 Jonathan Beezley <jon.beezley@gmail.com>

This file is part of libncpcolor.

libncpcolor is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libncpcolor is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser Public License for more details.

You should have received a copy of the GNU Lesser Public License
along with libncpcolor.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "lookupTable.h"
#include "lutData.h"

#include<iostream>
#include<dirent.h>
#include<string>
#include<cassert>

#undef NDEBUG

using namespace std;

int main() {
    LookupTable lutTable;
    struct dirent *entry;
    DIR *dp;
    string dirname("luts");
    int hasError = 0;
   
    const size_t N = 1000;
    uint8_t A[N];
    uint8_t *B = new uint8_t[LookupTable::imageSize(N)];
    
    for(size_t i=0; i<N; i++) A[i] = i % 256;
    dp = opendir(dirname.c_str());
    if (dp == NULL) {
        perror("Cannot open luts directory");
        return -1;
    }
    while( (entry = readdir(dp)) ) {
        string dname = entry->d_name;
        if(dname != "." && dname != ".." && dname.substr(dname.size()-2) == string(".h") ) {
            dname = dname.substr(0,dname.size()-2);
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
    cout << endl << "Loading jet" << endl;
    assert(lutTable.loadTable((string)"jet"));
    cout << endl << "Testing lutData.h:" << endl;
    for(int i = 0; i<lut::NTables; i++) {
        cout << "lut number " << i << ": " << lut::lookupTables[i].name << endl;
        lutTable.setReverse(false);
        lutTable.setData((uint8_t*) lut::lookupTables[i].data);
        lutTable.makePColor(100, A, B);
        lutTable.setReverse(true);
    }

    cout << endl << "Testing lutmap:" << endl;
    string lutname;
    for(int i = 0; i<lut::NTables; i++) {
        lutname = lut::lookupTables[i].name;
        cout << "Loading: " << lutname << endl;
        const LookupTable &lutRef = LookupTable::getLUT(lutname);
        lutRef.makePColor(100, A, B);
        cout << "Loading: " << lutname << " reversed" << endl;
        const LookupTable &lutRef_r = LookupTable::getLUT(lutname,true);
        lutRef_r.makePColor(100, A, B);
    }
    if(!hasError)
        cout << "Success!" << endl;
    delete [] B;
    return hasError;
}

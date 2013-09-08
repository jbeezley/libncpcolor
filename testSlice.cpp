
#undef NDEBUG

#include "ncFileReader.h"

#include<iostream>
#include<cassert>


using namespace std;

int main() {
    cout << "Testing nDims == 2\n";
    unsigned int n=2;
    SliceType::shapeType shape(n);
    shape[0] = 10;
    shape[1] = 7;
    SliceType s1(shape), s2(shape,0,1);
    assert(s1.isValid() && s2.isValid());

    s1[0] = 4;
    s1[1] = 5;
    cout << "Shape: " << shape[0] << " " << shape[1] << endl;
    long start[4], count[4];
    s1.getStart(start);
    s1.getCount(count);
    cout << "Start: " << start[0] << " " << start[1] << endl;
    cout << "Count: " << count[0] << " " << count[1] << endl;
    cout << "Transpose: " << s1.doTranspose() << endl;
    s2.getStart(start);
    s2.getCount(count);
    cout << "Start: " << start[0] << " " << start[1] << endl;
    cout << "Count: " << count[0] << " " << count[1] << endl;
    cout << "Transpose: " << s2.doTranspose() << endl;

    cout << "\nTesting nDims == 4\n";
    n=4;
    SliceType::shapeType s(n);
    s[0] = 10;
    s[1] = 20;
    s[2] = 15;
    s[3] = 5;
    SliceType s3(s);
    s3[0] = 0;
    s3[1] = 1;
    s3[2] = 2;
    s3[3] = 3;

    cout << "Shape: " << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << endl;
    for(unsigned int i = 0; i<n; i++) 
        for(unsigned int j = 0; j<n; j++) {
            if(i != j) {
    s3.setXDim(i);
    s3.setYDim(j);
    cout << "\nIndex: " << s3[0] << " " << s3[1] << " " << s3[2] << " " << s3[3] << endl;
    cout << "xDim: " << s3.xDim() << " yDim: " << s3.yDim() << endl;
    s3.getStart(start);
    s3.getCount(count);
    cout << "Start: " << start[0] << " " << start[1] << " " << start[2] << " " << start[3] << endl;
    cout << "Count: " << count[0] << " " << count[1] << " " << count[2] << " " << count[3] << endl;
    cout << "Transpose: " << s3.doTranspose() << endl;
    assert(s3.isValid());
            }
        }


}

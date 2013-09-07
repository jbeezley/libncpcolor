
#include "norm.h"

#include<cstdlib>
#include<stdint.h>
#include<iostream>

using namespace std;

int main() {
    LinearNorm<float> linnorm(-1.0, 1.0);
    LinearNorm<int> ilinnorm(0,100);
    LogNorm<double> lognorm(0.0,1.0);
    ilinnorm.setMinVal(10);
    ilinnorm.setMaxVal(100);
    lognorm.setMinVal(-1e-10);
    lognorm.setMaxVal(1.0);
    const size_t N = 10;
    int A[N];
    float B[N];
    double C[N];
    for(size_t i = 0; i<N; i++) {
        A[i] = i*11;
        B[i] = 2.0*(i-N/2.0)/N;
        C[i] = pow(10.0,(double) i - (double) N);
    }
    uint8_t Anorm[N], Bnorm[N], Cnorm[N];
    ilinnorm.normalize(N,A,Anorm);
    linnorm.normalize(N,B,Bnorm);
    lognorm.normalize(N,C,Cnorm);
    cout << "[ " << ilinnorm.minVal() << " , " << ilinnorm.maxVal() << " ]\n";
    cout << -1 << " -> " << (int) linnorm(-1) << endl;
    cout << "A[:] = ";
    for(size_t i = 0; i<N; i++) 
        cout << A[i] << " ";
    cout << endl;
    cout << "Anorm[:] = ";
    for(size_t i = 0; i<N; i++) 
        cout << (int) Anorm[i] << " ";
    cout << endl;
    cout << "[ " << linnorm.minVal() << " , " << linnorm.maxVal() << " ]\n";
    cout << -0.5 << " -> " << (int) linnorm(-0.5) << endl;
    cout << 101 << " -> " << (int) ilinnorm(101) << endl;
    cout << "B[:] = ";
    for(size_t i = 0; i<N; i++) 
        cout << B[i] << " ";
    cout << endl;
    cout << "Bnorm[:] = ";
    for(size_t i = 0; i<N; i++) 
        cout << (int) Bnorm[i] << " ";
    cout << endl;
    cout << "[ " << lognorm.minVal() << " , " << linnorm.maxVal() << " ]\n";
    cout << -0.5 << " -> " << (int) linnorm(-0.5) << endl;
    cout << 1.1 << " -> " << (int) lognorm(1.1) << endl;
    cout << 0.9 << " -> " << (int) lognorm(0.9) << endl;
    cout << "C[:] = ";
    for(size_t i = 0; i<N; i++) 
        cout << C[i] << " ";
    cout << endl;
    cout << "Cnorm[:] = ";
    for(size_t i = 0; i<N; i++) 
        cout << (int) Cnorm[i] << " ";
    cout << endl;
    return 0;
}

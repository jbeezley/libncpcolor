
#include "ncFileReader.h"

#include<iostream>
#include<string>
#include<typeinfo>

using namespace std;

int main() {
	string fileName("test.nc");
	NcSliceFile file(fileName);
	cout << file;
	
	for(NcSliceFile::variableMapType::const_iterator it = file.variables().begin();
		it != file.variables().end(); it++){
		
		BaseVariable *var = it->second;
		BaseVariable::sliceType slice = var->defaultSlice();
		cout << var->name() << var->shape() << slice << endl;
		double *A = new double[var->sliceSize(slice)];
		assert(var->readSlice(slice,A));
        if(var->nDims() > 2) {
		    slice.setXDim(0);
		    slice.setYDim(1);
		    delete [] A;
		    A = new double[var->sliceSize(slice)];
		    cout << var->name() << var->shape() << slice << endl;
		    assert(var->readSlice(slice,A));
        }
		delete [] A;
	}
}

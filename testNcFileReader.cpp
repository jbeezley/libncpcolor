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

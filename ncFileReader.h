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



#ifndef _NC_FILEREADER_H
#define _NC_FILEREADER_H

#include<cstdlib>
#include<vector>
#include<cassert>
#include<string>
#include<map>
#include<typeinfo>

#include "netcdfCompat.h"

using namespace std;

class SliceType : public vector<size_t> {
private:
    const vector<size_t> _shape;
    unsigned int _xDim, _yDim;
    SliceType() {}
public:
    
    typedef vector<size_t> shapeType;

    SliceType(const shapeType& shape,
              const unsigned int xDim,
              const unsigned int yDim) :
        vector<size_t>(shape.size(), 0), _shape(shape), 
        _xDim(xDim), _yDim(yDim) { assert(shape.size() >= 2); }
    
    SliceType(const shapeType& shape) :
        vector<size_t>(shape.size(), 0), _shape(shape) {
        assert(shape.size() >= 2);
        _xDim = size() - 1;
        _yDim = size() - 2;
    }

    bool isValid() const {
        return ( (size() >= 2) && (xDim() < size()) && (yDim() < size()) && (xDim() != yDim()) );
    }
    void setXDim(const unsigned int xDim) { _xDim = xDim; }
    void setYDim(const unsigned int yDim) { _yDim = yDim; }
    unsigned int xDim() const { return _xDim; }
    unsigned int yDim() const { return _yDim; }
    shapeType shape() const { return _shape; }

    void getStart(long* start) const;
    void getCount(long* count) const;
    bool doTranspose() const { return xDim() < yDim(); }
};

class BaseVariable {
protected:
    NcVar *_var;
    NcVar* var() const { return _var; }
    BaseVariable() { _var=NULL; }

public:
    typedef SliceType sliceType;
    typedef sliceType::shapeType shapeType;

    BaseVariable(NcVar *var) : _var(var) {}
    BaseVariable(const BaseVariable& v) : _var(v._var) {}
    
    virtual string myType() const { return (string) "void"; }

    unsigned int nDims() const {return var()->num_dims();}
    string dimName(int i) const {return (string) var()->get_dim(i)->name(); }
    shapeType shape() const;
    size_t size() const;
    size_t sliceSize(const sliceType& slice) const;
    string name() const { return (string) var()->name(); }
    sliceType defaultSlice() const { return sliceType(shape()); }
    virtual bool readSlice(const sliceType& slice, double* a) const = 0;
};

template<typename T>
class Variable : public BaseVariable {
private:
    virtual void transpose(const sliceType& slice, double* A) const;
public:
    typedef T varType;

    Variable(NcVar *var) : BaseVariable(var) {}
    Variable(const BaseVariable& v) : BaseVariable(v) {}
    
    virtual string myType() const { return (string) typeid(T).name(); }
    virtual bool readSlice(const sliceType& slice, double* a) const;
};

typedef Variable<double> VariableDouble;
typedef Variable<float> VariableFloat;
typedef Variable<int> VariableInt;
typedef Variable<long> VariableLong;
typedef Variable<short> VariableShort;
typedef Variable<ncbyte> VariableByte;

class NcSliceFile {
public:
    typedef map<string, BaseVariable*> variableMapType;
private:
    const NcFile _file;
    const string _fileName;
    variableMapType _variables;

    NcSliceFile() : _file("") { assert(0); } // should never be called

    static bool canDisplay(const NcVar* var);
public:
    
    NcSliceFile(const string& fileName);
    NcSliceFile(const NcSliceFile& file);
    ~NcSliceFile();
    bool isOpen() const;
    const variableMapType& variables() const { assert(isOpen()); return _variables; }
    string fileName() const { return _fileName; }
    const BaseVariable* getVariable(const string& varname) const;
};

ostream& operator<<(ostream& out, const BaseVariable& var);
ostream& operator<<(ostream& out, const NcSliceFile& file);
ostream& operator<<(ostream& out, const SliceType::shapeType& shape);
ostream& operator<<(ostream& out, const SliceType& slice);

#endif

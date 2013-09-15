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

/*!
 *  \file
 *
 *  \brief Defines classes to extract 2D slices from netCDF files.
 *
 *  This header defines several classes simplifying the reading of
 *  2 dimensional slices from variables in netCDF files.  
 */

class SliceType : public std::vector<size_t> {
private:
    const std::vector<size_t> _shape;
    unsigned int _xDim, _yDim;
    SliceType() {}
public:
    
    typedef std::vector<size_t> shapeType;

    SliceType(const shapeType& shape,
              const unsigned int xDim,
              const unsigned int yDim) :
        std::vector<size_t>(shape.size(), 0), _shape(shape), 
        _xDim(xDim), _yDim(yDim) { assert(shape.size() >= 2); }
    
    SliceType(const shapeType& shape) :
        std::vector<size_t>(shape.size(), 0), _shape(shape) {
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
    
    virtual std::string myType() const { return (std::string) "void"; }

    unsigned int nDims() const {return var()->num_dims();}
    std::string dimName(int i) const {return (std::string) var()->get_dim(i)->name(); }
    shapeType shape() const;
    size_t size() const;
    size_t sliceSize(const sliceType& slice) const;
    std::string name() const { return (std::string) var()->name(); }
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
    
    virtual std::string myType() const { return (std::string) typeid(T).name(); }
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
    typedef std::map<std::string, BaseVariable*> variableMapType;
private:
    const NcFile _file;
    const std::string _fileName;
    variableMapType _variables;

    NcSliceFile() : _file("") { assert(0); } // should never be called

    static bool canDisplay(const NcVar* var);
public:
    
    NcSliceFile(const std::string& fileName);
    NcSliceFile(const NcSliceFile& file);
    ~NcSliceFile();
    bool isOpen() const;
    const variableMapType& variables() const { assert(isOpen()); return _variables; }
    std::string fileName() const { return _fileName; }
    const BaseVariable* getVariable(const std::string& varname) const;
};

std::ostream& operator<<(std::ostream& out, const BaseVariable& var);
std::ostream& operator<<(std::ostream& out, const NcSliceFile& file);
std::ostream& operator<<(std::ostream& out, const SliceType::shapeType& shape);
std::ostream& operator<<(std::ostream& out, const SliceType& slice);

#endif

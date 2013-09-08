
#ifndef _NC_FILEREADER_H
#define _NC_FILEREADER_H

#include<cstdlib>
#include<vector>
#include<netcdfcpp.h>
#include<cassert>

using namespace std;

class SliceType : public vector<size_t> {
private:
    unsigned int _xDim, _yDim;
    const vector<size_t> _shape;
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

    void getStart(size_t* start) const;
    void getCount(size_t* count) const;
    bool doTranspose() const { return xDim() < yDim(); }
};

template<typename T>
class Variable {
private:
    NcVar *_var;
    Variable() { _var=NULL; }

public:
    typedef SliceType sliceType;
    typedef sliceType::shapeType shapeType;

    Variable(NcVar *var) : _var(var) {}
    Variable(const Variable& v) : _var(v._var) {}

    unsigned int nDims() const;
    shapeType shape() const;
    size_t size() const;

    size_t sliceSize(const sliceType& slice) const;
    T* allocArrayForSlice(const sliceType& slice) const {return new T[sliceSize(slice)];}
    void deAllocArray(T* a) const {if(a) delete [] a;}

    bool readSlice(const sliceType& slice, T* a) ; // can't be const because of NcVar interface
};

#endif

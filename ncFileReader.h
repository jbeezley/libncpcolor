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

/*! \class SliceType
 * \brief Defines a 2D slice from a 2+ dimension variable.
 *
 * This class describes an arbitrary 2D slice of a variable.
 * The private member _shape stores the shape of the variable,
 * while the contents of the vector itself (as a subclass of
 * std::vector) contain the indices of non-sliced dimensions.
 * Note that the sizes of these two vectors must always be the
 * same.
 *
 * For example, a 3D variable of shape [10, 11, 12] would have the
 * slice [:,:,2] stored as follows:
 *    _shape[] = [10, 11, 12]
 *    this[] = [-, -, 2]
 * where the '-' elements are arbitrary and ignored.  If one had the 
 * full variable A[:,:,:], the slice would then be A[0:9,0:10,2].  
 *
 * This class has member functions that are designed to work with the
 * netCDF interface for reading "hyperslabs" of variables (see netCDF
 * documentation for nc_get_vara_*(...)):
 *   * getStart(long*)
 *   * getCount(long*)
 *
 * Further, the class keeps track of which index is meant to be
 * displayed on the horizontal axis, and which index is meant to 
 * be displayed on the vertical axis.  This tells the reader class
 * BaseVariable whether to transpose the output or not.
 */
class SliceType : public std::vector<size_t> {
private:
    const std::vector<size_t> _shape; /*!< The shape of the variable. */
    unsigned int _xDim,  /*!< The index of the x-dimension. */
                 _yDim;  /*!< The index of the y-dimension. */ 
    SliceType() {}       /*!< The default constructor is private, because the _shape must
                          *   always be defined. */
public:
    
    typedef std::vector<size_t> shapeType; /*!< The storage type of variable "shapes".*/
    
    /*! The standard constructor.  The variable must be at least 2 dimensional.
     * \param[in] shape The shape of the variable (type SliceType::shapeType).
     * \param[in] xDim The index of the x-dimension (horizontal).
     * \param[in] yDim The index of the y-dimension (vertical).*/
    SliceType(const shapeType& shape,
              const unsigned int xDim,
              const unsigned int yDim) :
        std::vector<size_t>(shape.size(), 0), _shape(shape),
        _xDim(xDim), _yDim(yDim) { assert(shape.size() >= 2); }
    
    /*! A constructor similar to SliceType(const shapeType&, const unsigned int, const unsigned int)
     * where the x-dimension defaults to the last dimension and the y-dimension defaults to the
     * second to last dimension.
     * \sa SliceType(const shapeType&, const unsigned int, const unsigned int) */
    SliceType(const shapeType& shape) :
        std::vector<size_t>(shape.size(), 0), _shape(shape) {
        assert(shape.size() >= 2);
        _xDim = size() - 1;
        _yDim = size() - 2;
    }
    
    /*! Verify that the slice is valid.  In particular, the x and y dimensions must be
     *  less than size(), and the indices of non-sliced dimensions must be valid.
     *  \retval true If the object is valid
     *  \retval false If the object is invalid*/
    bool isValid() const {
        bool val = (size() >= 2) && (xDim() < size()) && (yDim() < size()) && (xDim() != yDim());
        for(unsigned int i=0; i<size(); i++) 
            if(i != xDim() && i != yDim()) 
                val &= this->operator[](i) < _shape[i];
        return val;
    }

    /*! Set the index of the x-dimension.
     * \sa xDim() */
    void setXDim(const unsigned int xDim) { _xDim = xDim; }
    
    /*! Set the index of the y-dimension.
     * \sa yDim()*/
    void setYDim(const unsigned int yDim) { _yDim = yDim; }

    /*! Get the index of the x-dimension.
     * \sa setXDim() */
    unsigned int xDim() const { return _xDim; }
    
    /*! Get the index of the y-dimension.
     * \sa setYDim() */
    unsigned int yDim() const { return _yDim; }

    /*! Get the shape of the variable. */
    const shapeType& shape() const { return _shape; }
    
    /*! Get an array of "start" indices as compatible with netCDF get_vara_*(...).
     * \param[out] start[] A pointer to an array of size size().  On output, start[i] contains
     * either 0 if i == xDim or yDim or this[i] otherwise.*/
    void getStart(long* start) const;

    /*! Get an array of "count" indices as compatible with netCDF get_vara_*(...).
     * \param[out] count[] A pointer to an array of size size().  On output, count[i] contains
     * either shape()[i] if i == xDim or yDim or 1 otherwise.*/
    void getCount(long* count) const;

    /*! Tells the reading class if the variable should be transposed or not.
     * \returns xDim() < yDim() */
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

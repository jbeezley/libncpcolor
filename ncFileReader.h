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
 * The private member \p _shape stores the shape of the variable,
 * while the contents of the vector itself (as a subclass of
 * <tt>std::vector</tt>) contain the indices of non-sliced dimensions.
 * Note that the sizes of these two vectors must always be the
 * same.
 *
 * For example, a 3D variable of shape <tt>[10, 11, 12]</tt> would have the
 * slice <tt>[:, :, 2]</tt> stored as follows:
 *
 *   * <tt>_shape[] = [10, 11, 12]</tt>
 *   * <tt>this[] = [-, -, 2]</tt>
 *
 * where the <tt>-</tt> elements are arbitrary and ignored.  If one had the 
 * full variable <tt>A[:,:,:]</tt>, the slice would then be <tt>A[0:9,0:10,2]</tt>.  
 *
 * This class has member functions that are designed to work with the
 * netCDF interface for reading "hyperslabs" of variables (see netCDF
 * documentation for <tt>nc_get_vara_*(...)</tt>):
 *   * getStart(long*) const
 *   * getCount(long*) const
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
     *  less than <tt>size()</tt>, and the indices of non-sliced dimensions must be valid.
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
    
    /*! Get an array of \p start indices as compatible with netCDF <tt>get_vara_*(...)</tt>.
     * \param[out] start[] A pointer to an array of size size().  On output, start[i] contains
     * either \p 0 if <tt>i == xDim</tt> or \p yDim or <tt>this[i]</tt> otherwise.*/
    void getStart(long* start) const;

    /*! Get an array of \p count indices as compatible with netCDF <tt>get_vara_*(...)</tt>.
     * \param[out] count[] A pointer to an array of size <tt>size()</tt>.  On output, <tt>count[i]</tt> contains
     * either <tt>shape()[i]</tt> if <tt>i == xDim</tt> or \p yDim or \p 1 otherwise.*/
    void getCount(long* count) const;

    /*! Tells the reading class if the variable should be transposed or not.
     * \returns xDim() < yDim() */
    bool doTranspose() const { return xDim() < yDim(); }
};

/*! \class BaseVariable
 *  \brief A simplified wrapper around a netCDF variable object.
 *
 *  This is an abstract base class defining a netCDF variable.  It's
 *  main purpose is to store the following:
 *
 *  * The variable's name
 *  * The variable's shape and total size
 *  * The names of the variable's dimensions
 *
 *  The class contains the method readSlice which takes in a SliceType object
 *  and returns the corresponding data from the netCDF file.*/
class BaseVariable {
    BaseVariable() { _var=NULL; } /*!< Default constructor should never be called. */
    NcVar *_var; /*!< Pointer to a netCDF variable. */

protected:
    NcVar* var() const { return _var; } /*!< Returns a pointer to the netCDF variable.*/

public:
    typedef SliceType sliceType;        
    typedef sliceType::shapeType shapeType;
    
    /*! Constructor
     * \param var Pointer to a netCDF variable */
    BaseVariable(NcVar *var) : _var(var) {}

    /*! Copy constructor */
    BaseVariable(const BaseVariable& v) : _var(v._var) {}
    
    /*! Returns a string describing the type of variable. */
    virtual std::string myType() const { return (std::string) "void"; }
    
    /*! Returns the number of dimensions of the variable. */
    unsigned int nDims() const {return var()->num_dims();}

    /*! Returns the name of the given dimension. 
     * \param[in] i The index of the requested dimension */
    std::string dimName(int i) const {return (std::string) var()->get_dim(i)->name(); }
    
    /*! Returns the shape of the variable. */
    shapeType shape() const;
    
    /*! Returns the total size of the variable. */
    size_t size() const;
    
    /*! Returns the total size of the given slice. */
    size_t sliceSize(const sliceType& slice) const;
    
    /*! Returns the name of the variable. */
    std::string name() const { return (std::string) var()->name(); }
    
    /*! Returns the "default" slice.  This is equivalent to BaseVariable::sliceType(shape()).*/
    sliceType defaultSlice() const { return sliceType(shape()); }
    
    /*! Reads the 2D slice indicated.  Must be reimplemented in the subclass to handle different
     * data types.
     * \param[in] slice The 2D slice requested.
     * \param[out] a An allocated array of size <tt>sliceSize(slice)</tt>.
     * \retval true if the read was successful.
     * \retval false if there was an error.*/
    virtual bool readSlice(const sliceType& slice, double* a) const = 0;
};

/*! \class Variable
 *  \brief A templated subclass of BaseVariable with a specific type. 
 *
 *  \tparam T The type of the variable as stored in the netCDF file.
 *
 *  Originally, this was designed to provide a virtual overload to readSlice() to
 *  prevent data type conversion; however, C++ does not allow such constructs
 *  without excessive use of select-case statements.  The remnants of this design
 *  remain but may change in future versions.*/
template<typename T>
class Variable : public BaseVariable {
private:
    /*! Perform a transpose operation on A if necessary. */
    virtual void transpose(const sliceType& slice, double* A) const;
public:
    /*! The templated type of this variable. */
    typedef T varType;
    
    /*! Constructor.
     *  \sa BaseVariable::BaseVariable(NcVar*)*/
    Variable(NcVar *var) : BaseVariable(var) {}
    
    /*! Copy constructor.
     *  \sa BaseVariable::BaseVariable(const BaseVariable&)*/
    Variable(const BaseVariable& v) : BaseVariable(v) {}
    
    virtual std::string myType() const { return (std::string) typeid(T).name(); }
    virtual bool readSlice(const sliceType& slice, double* a) const;
};

/*! Default type of variable.  Will always convert data in the netCDF file to doubles.*/
typedef Variable<double> VariableDouble;
/*! Single precision floating point variable. */
typedef Variable<float> VariableFloat;
/*! Standard int integer variable. */
typedef Variable<int> VariableInt;
/*! Standard long integer variable. */
typedef Variable<long> VariableLong;
/*! Standard short integer variable. */
typedef Variable<short> VariableShort;
/*! Standard single byte integer variable. */
typedef Variable<ncbyte> VariableByte;

/*! \class NcSliceFile
 *  \brief A simplified wrapper around a netCDF file object. 
 *
 *  The primary purpose of this class is to list the "compatible"
 *  variables contained in a netCDF file, where a compatible variable
 *  is one which is at least two dimensional and is of simple numeric
 *  type.  Upon successfully opening a netCDF file, the class constructs
 *  ::VariableDouble objects for each compatible variable found and places
 *  them inside a <tt>std::map</tt>.*/
class NcSliceFile {
public:
    /*! Convience typedef of the mapping from variable name to variable object. */
    typedef std::map<std::string, BaseVariable*> variableMapType;
private:
    const NcFile _file;
    const std::string _fileName;
    variableMapType _variables;

    NcSliceFile() : _file("") { assert(0); } // should never be called

    static bool canDisplay(const NcVar* var);
public:
    /*! Standard constructor to open a netCDF file.  One should always check if
     * the file was opened correctly by calling the isOpen() method after construction.
     * \param[in] fileName The name of the file to open*/    
    NcSliceFile(const std::string& fileName);

    /*! Copy constructor */
    NcSliceFile(const NcSliceFile& file);

    /*! Standard destructor... frees all allocated memory and closes the file. */
    ~NcSliceFile();

    /*! Test if the file is open.
     * \retval true The file is open and variables can be accessed. 
     * \retval false There was an error opening the file.  Variables cannot be accessed.*/
    bool isOpen() const;

    /*! Return a <tt>std::map</tt> object that contains all of the "compatible" variables in the file. 
     *  The file must be open when this call is made.
     *  \returns NcSliceFile::variableMapType \p varMap, where <tt>varMap[varName]</tt> is a pointer to the variable, <tt>varName</tt>.
     *  \sa isOpen()*/
    const variableMapType& variables() const { assert(isOpen()); return _variables; }

    /*! Returns the name of the file. */
    std::string fileName() const { return _fileName; }

    /*! Returns a pointer to the given variable.  Equivalent to <tt>variables()[varname]</tt>*/
    const BaseVariable* getVariable(const std::string& varname) const;
};

/*! For debugging, write a description of the variable to a stream. */
std::ostream& operator<<(std::ostream& out, const BaseVariable& var);
/*! For debugging, write a description of the file to a stream. */
std::ostream& operator<<(std::ostream& out, const NcSliceFile& file);
/*! For debugging, write a description of a variable shape to a stream. */
std::ostream& operator<<(std::ostream& out, const SliceType::shapeType& shape);
/*! For debugging, write a description of a slice to a stream. */
std::ostream& operator<<(std::ostream& out, const SliceType& slice);

#endif

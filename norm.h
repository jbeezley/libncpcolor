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


/*!
 * \file
 *
 * \brief Normalize abstract data to uint8_t
 *
 * \author Jonathan Beezley <jon.beezley@gmail.com>
 *
 * Header defining a simple template class that normalizes values as an unsigned
 * 8 bit integer.  The LinearNorm class defines an affine linear mapping:
 *
 *     [minVal(),maxVal()] -> [0,255]
 *
 * Values outside of the range [minVal(),maxVal()] are clipped into it.
 *
 * The operator () maps the argument using this mapping.
 * A convience method normalize() performs the mapping to each element of an array.
*/

#ifndef _NORM_H
#define _NORM_H

#include<cstdlib>
#include<stdint.h>
#include<cmath>


/*! \class LinearNorm
 *  \brief Defines an affine mapping from [minValue(),maxVal()] -> [0,255].
 *  \tparam T specifies the type that the norm acts on... the norm itself always returns a uint8_t.*/
template <typename T>
class LinearNorm {
private:
    /*! Private members defining the mapping.*/
    T _minVal, /*!< Minimum value of the mapping. */
      _maxVal, /*!< Maximum value of the mapping. */
      _range;  /*!< Range of the mapping (_maxVal - _minVal) */

protected:
    /*! Designed to be implemented by subclasses.  Here we clip values outside the domain.*/
    virtual double scale(const double x) const { return fmin(fmax(x,0.0),1.0); }

    /*! Set the _range member, which is dependent on min and max.  To avoid NaN, 
     *  the range is set to 1.0 when min == max.*/
    static double computeRange(const T& min, const T& max) {
        double r = (double) max - min;
        if (r <= 0.0) r = 1.0;
        return r;
    }
public:

    /*! Default constructor. */
    LinearNorm() {}

    /*! Construct a well defined mapping from min/max value. */
    /*!
     * \param[in] minVal the minimal value of the mapping ( minVal |-> 0 )
     * \param[in] maxVal the maximum value of the mapping ( maxVal |-> 255 )
    */
    LinearNorm(const T& minVal, const T& maxVal) : _minVal(minVal),_maxVal(maxVal) {
        _range = LinearNorm<T>::computeRange(_minVal, _maxVal);
    }

    /*! Copy constructor. */
    LinearNorm(const LinearNorm<T>& other) : _minVal(other._minVal), 
                                             _maxVal(other._maxVal),
                                             _range(other._range) {}

    /*! Set the minimum value of the mapping. */
    void setMinVal(const T& minVal) {
        _minVal = minVal;
        _range = LinearNorm<T>::computeRange(_minVal, _maxVal);
    }
    
    /*! Set the maximum value of the mapping. */
    void setMaxVal(const T& maxVal) {
        _maxVal = maxVal;
        _range = LinearNorm<T>::computeRange(_minVal, _maxVal);
    }

    /*! Returns the minimum value of the mapping. */
    T minVal() const { return _minVal; }
    
    /*! Returns the maximum value of the mapping. */
    T maxVal() const { return _maxVal; }
    
    /*! Returns the range of the mapping. (maxVal() - minVal())*/
    T range() const { return _range; }
    
    /*! Convenience method for setting minimum value according to an array. 
     * \param[in] N size of the array
     * \param[in] Array[] pointer to an array of size N.
     * \sa setMaxValFromArray()
     * \sa setMinMaxValFromArray()*/
    void setMinValFromArray(const size_t N, const T Array[]) {
        T minVal = Array[0];
        for(size_t i = 1; i<N; i++) if(Array[i] < minVal) minVal = Array[i];
        setMinVal(minVal);
    }
    
    /*! Convenience method for setting maximum value according to an array. 
     * \param[in] N size of the array
     * \param[in] Array[] pointer to an array of size N.
     * \sa setMinValFromArray()
     * \sa setMinMaxValFromArray()*/
    void setMaxValFromArray(const size_t N, const T Array[]) {
        T maxVal = Array[0];
        for(size_t i = 1; i<N; i++) if(Array[i] > maxVal) maxVal = Array[i];
        setMaxVal(maxVal);
    }
    
    /*! Convenience method for setting minimum/maximum value according to an array. 
     * \param[in] N size of the array
     * \param[in] Array[] pointer to an array of size N.
     * \sa setMinValFromArray()
     * \sa setMaxValFromArray()*/
    void setMinMaxValFromArray(const size_t N, const T Array[]) {
        T minVal = Array[0];
        T maxVal = Array[0];
        for(size_t i = 1; i<N; i++) {
            if(Array[i] < minVal) minVal = Array[i];
            else if(Array[i] > maxVal) maxVal = Array[i];
            setMinVal(minVal);
            setMaxVal(maxVal);
        }
    }

    /*! Perform the mapping on a scalar or array. 
     * \param[in] val scalar of type T.
     * \return uint8_t normalized value.
     * \sa normalize()*/
    virtual uint8_t operator() (const T& val) const {return (uint8_t) round(255.0 * scale(((double)val - (double)minVal())/range()));}

    /*! Normalize an array. 
     * \param[in] N size of the arrays.
     * \param[in] inArray[] T array of size N to normalize.
     * \param[out] outArray[] uint8_t array of size N containing the normalized values. 
     * \sa operator()*/
    virtual void normalize(const size_t N, const T inArray[], uint8_t outArray[]) const {
        for(size_t i = 0; i<N; i++) outArray[i] = (*this)(inArray[i]);
    }
};

#pragma GCC diagnostic ignored "-Wunused-parameter"
/*! \class LogNorm
 *  \brief Meant to be Log scaling normalization, but it is broken at the moment.
 *  \tparam T specifies the type that the norm acts on... the norm itself always returns a uint8_t.*/
template <typename T>
class LogNorm : public LinearNorm<T> {
private:
    virtual double scale(const double x) const { 
        T m = this->minVal();
        double s = -1.0 * log2((double) m + _minCutoff);
        return LinearNorm<T>::scale( log2(fmax(_minCutoff,x))/s  + 1.0 ); 
    }
    static const double _minCutoff;
public:
    LogNorm() {}
    LogNorm(const T& minVal, const T& maxVal) : LinearNorm<T>(minVal,maxVal) {
//        LinearNorm<T>::setMinVal(fmax(_minCutoff,minVal));
    }
    LogNorm(const LogNorm<T>& other) : LinearNorm<T>(other._minVal,other._maxVal) {}
    void setMinVal(const T& minVal) {
        //LinearNorm<T>::setMinVal(fmax(_minCutoff,minVal));
    }
};

template <typename T>
const double LogNorm<T>::_minCutoff = 1e-14;
#endif

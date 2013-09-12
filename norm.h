
/* Header defining a simple template class that normalizes values as an unsigned
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


template <typename T>
class LinearNorm {
private:
    /* Private members defining the mapping.*/
    T _minVal, _maxVal, _range;
protected:
    /* Designed to be implemented by subclasses.  Here we clip values outside the domain.*/
    virtual double scale(const double x) const { return fmin(fmax(x,0.0),1.0); }

    /* Set the _range member, which is dependent on min and max.  To avoid NaN, 
     * the range is set to 1.0 when min == max.*/
    static double computeRange(const T& min, const T& max) {
        double r = (double) max - min;
        if (r <= 0.0) r = 1.0;
        return r;
    }
public:
    /* Constructors:
     *   LinearNorm norm;
     *   LinearNorm norm(minVal, maxVal);
     *   LinearNorm norm(otherNorm);
     */
    LinearNorm() {}
    LinearNorm(const T& minVal, const T& maxVal) : _minVal(minVal),_maxVal(maxVal) {
        _range = LinearNorm<T>::computeRange(_minVal, _maxVal);
    }
    LinearNorm(const LinearNorm<T>& other) : LinearNorm<T>(other._minVal,other._maxVal) {}

    /* Public access to minimum and maximum values of the mapping. */
    void setMinVal(const T& minVal) {
        _minVal = minVal;
        _range = LinearNorm<T>::computeRange(_minVal, _maxVal);
    }
    void setMaxVal(const T& maxVal) {
        _maxVal = maxVal;
        _range = LinearNorm<T>::computeRange(_minVal, _maxVal);
    }
    T minVal() const { return _minVal; }
    T maxVal() const { return _maxVal; }
    T range() const { return _range; }
    
    /* Convenience methods for setting minimum and maximum values according to an array. */
    void setMinValFromArray(const size_t N, const T Array[]) {
        T minVal = Array[0];
        for(size_t i = 1; i<N; i++) if(Array[i] < minVal) minVal = Array[i];
        setMinVal(minVal);
    }
    void setMaxValFromArray(const size_t N, const T Array[]) {
        T maxVal = Array[0];
        for(size_t i = 1; i<N; i++) if(Array[i] > maxVal) maxVal = Array[i];
        setMaxVal(maxVal);
    }
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

    /* Perform the mapping on a scalar or array. */
    virtual uint8_t operator() (const T& val) const {return (uint8_t) round(255.0 * scale(((double)val - (double)minVal())/range()));}
    virtual void normalize(const size_t N, const T inArray[], uint8_t outArray[]) const {
        for(size_t i = 0; i<N; i++) outArray[i] = (*this)(inArray[i]);
    }
};

#pragma GCC diagnostic ignored "-Wunused-parameter"
/* Meant to be Log scaling normalization, but it is broken at the moment. */
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
    LogNorm(const LogNorm<T>& other) : LogNorm<T>(other._minVal,other._maxVal) {}
    void setMinVal(const T& minVal) {
        //LinearNorm<T>::setMinVal(fmax(_minCutoff,minVal));
    }
};

template <typename T>
const double LogNorm<T>::_minCutoff = 1e-14;
#endif

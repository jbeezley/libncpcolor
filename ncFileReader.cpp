
#include "ncFileReader.h"

void SliceType::getStart(size_t* start) const {
    assert (isValid());
    for(unsigned int i=0; i<size(); i++) {
        if(i == xDim())
            start[i] = 0;
        else if(i == yDim())
            start[i] = 0;
        else
            start[i] = (*this)[i];
    }
}

void SliceType::getCount(size_t* count) const {
    assert (isValid());
    for(unsigned int i=0; i<size(); i++) {
        if(i == xDim())
            count[i] = shape()[i];
        else if(i == yDim())
            count[i] = shape()[i];
        else
            count[i] = 1;
    }
}


#include "ncFileReader.h"

void SliceType::getStart(long* start) const {
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

void SliceType::getCount(long* count) const {
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

template class Variable<double>;
template class Variable<float>;
template class Variable<int>;
template class Variable<long>;
template class Variable<short>;
template class Variable<ncbyte>;

BaseVariable::shapeType BaseVariable::shape() const {
    shapeType s(nDims());
    for(unsigned int i=0; i<nDims(); i++) s[i] = var()->get_dim(i)->size();
    return s;
}

size_t BaseVariable::size() const {
    shapeType s = shape();
    size_t n=1; 
    for(unsigned int i=0; i<nDims(); i++) n = n*s[i];
    return n;
}

size_t BaseVariable::sliceSize(const BaseVariable::sliceType& slice) const {
    long count[nDims()];
    slice.getCount(count);
    size_t N=1;
    for(unsigned int i=0; i<nDims(); i++) N = N*count[i];
    return N;
}

template<typename T>
bool Variable<T>::readSlice(const Variable<T>::sliceType& slice, double* a) const {
    long count[nDims()], start[nDims()];
    slice.getStart(start);
    slice.getCount(count);
    var()->set_cur(start);
    var()->get(a, count);
    transpose(slice, a);
    return true;
}

template<typename T>
void Variable<T>::transpose(const sliceType& slice, double* A) const {
    if(slice.doTranspose())
    {
        double tmp;
        const size_t ny = shape()[slice.yDim()];
        const size_t nx = shape()[slice.xDim()];
        for(size_t i=0; i<slice.yDim(); i++)
            for(size_t j=0; j<slice.xDim(); j++) {
                tmp = A[i*nx + j];
                A[i*nx + j] = A[j*ny + i];
                A[j*ny + i] = tmp;
            }
    }
}

NcSliceFile::NcSliceFile(const string& fileName) : _file(fileName.c_str()), _fileName(fileName) {
    for(int i = 0; i<_file.num_vars(); i++) {
        NcVar *var = _file.get_var(i);
        if(canDisplay(var)) {
            NcType type = var->type();
            BaseVariable *baseVar;
            string name(var->name());
            if(type == ncByte)
                baseVar = new VariableByte(var);
            else if(type == ncInt)
                baseVar = new VariableInt(var);
            else if(type == ncLong)
                baseVar = new VariableLong(var);
            else if(type == ncFloat)
                baseVar = new VariableFloat(var);
            else if(type == ncDouble)
                baseVar = new VariableDouble(var);
            _variables[name] = baseVar;
        }
    }
}

NcSliceFile::NcSliceFile(const NcSliceFile& file) : _file(file._fileName.c_str()), _fileName(file._fileName) {
    for(int i = 0; i<_file.num_vars(); i++) {
        NcVar *var = _file.get_var(i);
        if(canDisplay(var)) {
            NcType type = var->type();
            BaseVariable *baseVar;
            string name(var->name());
            if(type == ncByte)
                baseVar = new VariableByte(var);
            else if(type == ncInt)
                baseVar = new VariableInt(var);
            else if(type == ncLong)
                baseVar = new VariableLong(var);
            else if(type == ncFloat)
                baseVar = new VariableFloat(var);
            else if(type == ncDouble)
                baseVar = new VariableDouble(var);
            else
                assert(0);
            _variables[name] = baseVar;
        }
    }
}

bool NcSliceFile::isOpen() const {
    return _file.is_valid();
}

NcSliceFile::~NcSliceFile() {
//    for(variableMapType::iterator var=_variables.begin(); var!=_variables.end(); ++var)
//        delete var->second;
}

bool NcSliceFile::canDisplay(const NcVar* var) {
    NcType type = var->type();
    int nDims = var->num_dims();

    return ( (nDims >= 2) && 
             (var->num_vals() > 0) && (
             (type == ncByte)  ||
             (type == ncShort) ||
             (type == ncInt)   ||
             (type == ncLong)  ||
             (type == ncFloat) ||
             (type == ncDouble) ) );
}

const BaseVariable* NcSliceFile::getVariable(const string& varname) const {
    assert(isOpen());
    const variableMapType vmap = variables();
    variableMapType::const_iterator var = vmap.find(varname);
    if(var == vmap.end())
        return NULL;
    else
        return var->second;
}

ostream& operator<<(ostream& out, const BaseVariable& var) {
    out << var.myType() << " " << var.name() << "[";
    if(var.nDims() > 0) out << var.shape()[0];
    for(int i = 1; i<var.nDims(); i++) out << ", " << var.shape()[i];
    out << "]";
    return out;
}

ostream& operator<<(ostream& out, const NcSliceFile& file) {
out << file.fileName() << " {" << endl;
for(NcSliceFile::variableMapType::const_iterator it=file.variables().begin();
    it != file.variables().end(); ++it) {
    out << "  " << *it->second << endl;
}
out << "}" << endl;
return out;
}

ostream& operator<<(ostream& out, const SliceType& slice) {
    out << "[";
    for(int i = 0; i< slice.size(); i++) {
        if(i == slice.xDim() || i == slice.yDim()) {
            out << "0:" << slice.shape()[i];
        }
        else {
            out << slice[i];
        }
        if(i<slice.size() - 1) out << ", ";
    }
    out << "]";
    return out;
}

ostream& operator<<(ostream& out, const SliceType::shapeType& shape) {
    out << "[";
    for(int i = 0; i<shape.size(); i++) {
        out << shape[i];
        if(i<shape.size() - 1) out << ", ";
    }
    out << "]";
    return out;
}

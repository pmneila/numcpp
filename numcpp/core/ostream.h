
#ifndef NUMCPP_OSTREAM_H
#define NUMCPP_OSTREAM_H

namespace numcpp
{

std::ostream& operator<< (std::ostream& os, const Shape& x)
{
    os << "(";
    for(auto& y: x)
        os << y << ",";
    os << ")";
    return os;
}

std::ostream& operator<< (std::ostream& os, const Strides& x)
{
    os << "(";
    for(auto& y: x)
        os << y << ", ";
    os << ")";
    return os;
}

template<class T>
std::ostream& operator<< (std::ostream& os, const Array<T>& x)
{
    os << "Array<" << typeid(T).name() << "> {\n";
    os << "  ndims = " << x.ndims() << "\n";
    os << "  shape = " << x.shape() << "\n";
    os << "  strides = " << x.strides() << "\n";
    os << "  offset = " << x.offset() << "\n";
    
    os << "  data = " << "\n";
    // size_t j = 0;
    // size_t nrRows = x.size() / x.shape(x.ndims()-1);
    // for(size_t i=0; i< nrRows; i++)
    // {
    //     for(size_t l=0; l < x.shape(x.ndims()-1); l++,j++)
    //         os << x[j] << " ";
    //     os << "\n";
    // }
    os << "}";
    return os;
}

}

#endif

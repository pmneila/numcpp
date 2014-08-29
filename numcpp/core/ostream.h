
#ifndef NUMCPP_OSTREAM_H
#define NUMCPP_OSTREAM_H

std::ostream& operator<< (std::ostream& os, const numcpp::Shape& x)
{
    os << "(";
    for(auto& y: x)
        os << y << ",";
    os << ")";
    return os;
}

std::ostream& operator<< (std::ostream& os, const numcpp::Strides& x)
{
    os << "(";
    for(auto& y: x)
        os << y << ", ";
    os << ")";
    return os;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& x)
{
    os << "(";
    for(auto& y: x)
        os << y << ", ";
    os << ")";
    return os;
}

std::ostream& operator<< (std::ostream& os, const numcpp::Index& x)
{
    static const std::vector<std::string> names = {"INT, SLICE, NEWAXIS"};
    os << "(";
    os << names[x.tag()] << ", ";
    switch(x.tag())
    {
    case 0:
        os << x.index();
        break;
    case 1:
        os << x.slice();
        break;
    case 2:
        os << ":";
    }
    os << ")";
    return os;
}

template<class T, class Derived>
std::ostream& operator<< (std::ostream& os, const numcpp::ArrayBase<T, Derived>& x)
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

#endif

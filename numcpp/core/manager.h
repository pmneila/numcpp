
#ifndef NUMCPP_MANAGER_H
#define NUMCPP_MANAGER_H

#include <memory>

namespace numcpp
{

class Manager
{
public:
    
    typedef std::shared_ptr<Manager> Ptr;
    
    Manager(const Manager&) = delete;
    
protected:
    explicit Manager() {}
    virtual ~Manager() {}
};

class SimpleManager : public Manager
{
public:
    
private:
    explicit SimpleManager(size_t size)
        : _array(new unsigned char[size])
    
    std::unique_ptr<unsigned char[]> _array;
};

}

#endif

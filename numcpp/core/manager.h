
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
    
    virtual void* data() const = 0;
    
protected:
    explicit Manager() {}
    virtual ~Manager() {std::cout << "Manager deleted" << std::endl;}
};

class SimpleManager : public Manager
{
private:
    std::unique_ptr<unsigned char[]> _array;
    
    explicit SimpleManager(size_t size)
        : _array(new unsigned char[size])
    {}
    
public:
    
    static Manager::Ptr allocate(size_t size)
    {
        Manager::Ptr r(new SimpleManager(size));
        return r;
    }
    
    virtual void* data() const {return _array.get();}    
};

}

#endif
